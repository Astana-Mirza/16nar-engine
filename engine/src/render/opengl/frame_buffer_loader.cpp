#include <16nar/render/opengl/frame_buffer_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

namespace
{

bool load_attachment( const LoadParams< ResourceType::FrameBuffer >::AttachmentParams& params,
     FrameBufferLoader::Handler& handler )
{
     bool success = true;
     unsigned int descriptor;
     switch ( params.storage )
     {
          case StorageType::RenderBuffer:
          {
               glGenRenderbuffers( 1, &descriptor );
               glBindRenderbuffer( GL_RENDERBUFFER, descriptor );
               if ( params.samples )
               {
                    glRenderbufferStorageMultisample( GL_RENDERBUFFER, params.samples,
                         default_internal_format( params.format, params.data_type ), params.size.x(), params.size.y() );
               }
               else
               {
                    glRenderbufferStorage( GL_RENDERBUFFER,
                         default_internal_format( params.format, params.data_type ),
                         params.size.x(), params.size.y() );
               }
               glBindRenderbuffer( GL_RENDERBUFFER, 0 );
               glFramebufferRenderbuffer( GL_FRAMEBUFFER,
                    attachment_type_to_int( params.attachment, params.order ), GL_RENDERBUFFER, descriptor );
          }
          break;
          case StorageType::Texture2D:
          {
               unsigned int texture_type;
               glGenTextures( 1, &descriptor );
               if ( params.samples )
               {
                    texture_type = GL_TEXTURE_2D_MULTISAMPLE;
                    glBindTexture( texture_type, descriptor );
                    glTexImage2DMultisample( texture_type, params.samples, data_format_to_int( params.format ),
                         params.size.x(), params.size.y(), GL_TRUE );
               }
               else
               {
                    texture_type = GL_TEXTURE_2D;
                    glBindTexture( texture_type, descriptor );
                    glTexImage2D( texture_type, 0, data_format_to_int( params.format ), params.size.x(), params.size.y(),
                         0, data_format_to_int( params.format ), data_type_to_int( params.data_type ), nullptr );
               }
               glTexParameteri( texture_type, GL_TEXTURE_MIN_FILTER, tex_filter_to_int( params.min_filter ) );
               glTexParameteri( texture_type, GL_TEXTURE_MAG_FILTER, tex_filter_to_int( params.mag_filter ) );
               glTexParameteri( texture_type, GL_TEXTURE_WRAP_S, tex_wrap_to_int( params.wrap_x ) );
               glTexParameteri( texture_type, GL_TEXTURE_WRAP_T, tex_wrap_to_int( params.wrap_y ) );
               glBindTexture( texture_type, 0 );
               glFramebufferTexture2D( GL_FRAMEBUFFER, attachment_type_to_int( params.attachment, params.order ),
                    texture_type, descriptor, 0 );
          }
          break;
          case StorageType::Texture3D:
          {
               glGenTextures( 1, &descriptor );
               glBindTexture( GL_TEXTURE_CUBE_MAP, descriptor );
               for ( unsigned int i = 0; i < 6; ++i )
               {
                    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         data_format_to_int( params.format ), params.size.x(), params.size.y(),
                         0, data_format_to_int( params.format ),
                         data_type_to_int( params.data_type ), nullptr );
               }
               glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, tex_filter_to_int( params.min_filter ) );
               glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, tex_filter_to_int( params.mag_filter ) );
               glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tex_wrap_to_int( params.wrap_x ) );
               glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tex_wrap_to_int( params.wrap_y ) );
               glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, tex_wrap_to_int( params.wrap_z ) );
               glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
               glFramebufferTexture( GL_FRAMEBUFFER, attachment_type_to_int( params.attachment, params.order ),
                    descriptor, 0 );
          }
          break;
          default:
          {
               LOG_16NAR_ERROR( "Wrong framebuffer attachment storage type to load: "
                    << static_cast< std::size_t >( params.storage ) );
               success = false;
          }
          break;
     }
     if ( success )
     {
          handler.attachments.push_back( { descriptor, params.storage } );
     }
     return success;
}


bool unload_attachments( const FrameBufferLoader::Handler& handler )
{
     bool success = true;
     for ( const auto& attachment : handler.attachments )
     {
          switch ( attachment.type )
          {
               case StorageType::RenderBuffer:
               {
                    glDeleteRenderbuffers( 1, &attachment.descriptor );
               }
               break;
               case StorageType::Texture2D:
               case StorageType::Texture3D:
               {
                    glDeleteTextures( 1, &attachment.descriptor );
               }
               break;
               default:
               {
                    LOG_16NAR_ERROR( "Wrong framebuffer attachment storage type to unload: "
                         << static_cast< std::size_t >( attachment.type ) );
                    success = false;
               }
               break;
          }
     }
     return success;
}

} // anonymous namespace


bool FrameBufferLoader::load( const LoadParams& params, Handler& handler )
{
     glGenFramebuffers( 1, &handler.descriptor );
     glBindFramebuffer( GL_FRAMEBUFFER, handler.descriptor );
     for ( std::size_t i = 0; i < params.attachments.size(); i++ )
     {
          if ( !load_attachment( params.attachments[ i ], handler ) )
          {
               unload_attachments( handler );
               glBindFramebuffer( GL_FRAMEBUFFER, 0 );
               glDeleteFramebuffers( 1, &handler.descriptor );
               return false;
          }
     }
     if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
     {
          LOG_16NAR_ERROR( "Unable to complete framebuffer" );
          unload_attachments( handler );
          glBindFramebuffer( GL_FRAMEBUFFER, 0 );
          glDeleteFramebuffers( 1, &handler.descriptor );
          return false;
     }
     glBindFramebuffer( GL_FRAMEBUFFER, 0 );
     LOG_16NAR_DEBUG( "Framebuffer " << handler.descriptor << " was loaded" );
     return true;
}


bool FrameBufferLoader::unload( const Handler& handler )
{
     bool success = unload_attachments( handler );
     glDeleteFramebuffers( 1, &handler.descriptor );
     LOG_16NAR_DEBUG( "Framebuffer " << handler.descriptor << " was unloaded "
          << ( success ? "successfully" : "with error" ) );
     return success;
}

} // namespace _16nar::opengl
