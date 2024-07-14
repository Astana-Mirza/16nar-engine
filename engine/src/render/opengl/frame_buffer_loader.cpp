#include <16nar/render/opengl/frame_buffer_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

namespace
{

bool load_attachment( const ResourceManagerMap& managers,
     const LoadParams< ResourceType::FrameBuffer >::AttachmentParams& params,
     FrameBufferLoader::HandlerType& handler,
     std::vector< unsigned int >& color_attachments )
{
     bool success = true;
     unsigned int descriptor;
     switch ( params.resource.type )
     {
          case ResourceType::RenderBuffer:
          {
               descriptor = std::any_cast< Handler< ResourceType::RenderBuffer > >(
                    managers.at( ResourceType::RenderBuffer )->get_handler( params.resource.id ) ).descriptor;
               glFramebufferRenderbuffer( GL_FRAMEBUFFER,
                    attachment_type_to_int( params.type, params.order ), GL_RENDERBUFFER, descriptor );
          }
          break;
          case ResourceType::Texture:
          {
               descriptor = std::any_cast< Handler< ResourceType::Texture > >(
                    managers.at( ResourceType::Texture )->get_handler( params.resource.id ) ).descriptor;
               unsigned int texture_type = params.multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
               glFramebufferTexture2D( GL_FRAMEBUFFER, attachment_type_to_int( params.type, params.order ),
                    texture_type, descriptor, 0 );
          }
          break;
          case ResourceType::Cubemap:
          {
               descriptor = std::any_cast< Handler< ResourceType::Cubemap > >(
                    managers.at( ResourceType::Cubemap )->get_handler( params.resource.id ) ).descriptor;
               glFramebufferTexture( GL_FRAMEBUFFER, attachment_type_to_int( params.type, params.order ),
                    descriptor, 0 );
          }
          break;
          default:
          {
               LOG_16NAR_ERROR( "Wrong framebuffer attachment resource type to load: "
                    << static_cast< std::size_t >( params.resource.type ) );
               success = false;
          }
          break;
     }
     if ( success )
     {
          color_attachments.push_back( attachment_type_to_int( params.type, params.order ) );
     }
     return success;
}

} // anonymous namespace


bool FrameBufferLoader::load( const ResourceManagerMap& managers, const LoadParamsType& params, HandlerType& handler )
{
     std::vector< unsigned int > color_attachments;
     glGenFramebuffers( 1, &handler.descriptor );
     glBindFramebuffer( GL_FRAMEBUFFER, handler.descriptor );
     for ( std::size_t i = 0; i < params.attachments.size(); i++ )
     {
          if ( !load_attachment( managers, params.attachments[ i ], handler, color_attachments ) )
          {
               glBindFramebuffer( GL_FRAMEBUFFER, 0 );
               glDeleteFramebuffers( 1, &handler.descriptor );
               return false;
          }
     }
     if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
     {
          LOG_16NAR_ERROR( "Unable to complete framebuffer" );
          glBindFramebuffer( GL_FRAMEBUFFER, 0 );
          glDeleteFramebuffers( 1, &handler.descriptor );
          return false;
     }
     glDrawBuffers( color_attachments.size(), color_attachments.data() );
     glBindFramebuffer( GL_FRAMEBUFFER, 0 );
     LOG_16NAR_DEBUG( "Framebuffer " << handler.descriptor << " was loaded" );
     return true;
}


bool FrameBufferLoader::unload( const HandlerType& handler )
{
     glDeleteFramebuffers( 1, &handler.descriptor );
     LOG_16NAR_DEBUG( "Framebuffer " << handler.descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
