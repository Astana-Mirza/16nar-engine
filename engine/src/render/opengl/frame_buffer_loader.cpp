#include <16nar/render/opengl/frame_buffer_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

#include <memory>

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
          // managers are handled by render API, so at() must never throw here
          case ResourceType::RenderBuffer:
          {
               std::any res_any = managers.at( ResourceType::RenderBuffer )->get_handler( params.resource.id );
               const auto *ptr = std::any_cast< Handler< ResourceType::RenderBuffer > >( &res_any );
               if ( !ptr )
               {
                    LOG_16NAR_ERROR( "Wrong handler for render buffer id " << params.resource.id );
                    return false;
               }
               descriptor = ptr->descriptor;
               glFramebufferRenderbuffer( GL_FRAMEBUFFER,
                    attachment_type_to_int( params.type, params.order ), GL_RENDERBUFFER, descriptor );
          }
          break;
          case ResourceType::Texture:
          {
               std::any res_any = managers.at( ResourceType::Texture )->get_handler( params.resource.id );
               const auto *ptr = std::any_cast< Handler< ResourceType::Texture > >( &res_any );
               if ( !ptr )
               {
                    LOG_16NAR_ERROR( "Wrong handler for texture id " << params.resource.id );
                    return false;
               }
               descriptor = ptr->descriptor;
               unsigned int texture_type = params.multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
               glFramebufferTexture2D( GL_FRAMEBUFFER, attachment_type_to_int( params.type, params.order ),
                    texture_type, descriptor, 0 );
          }
          break;
          case ResourceType::Cubemap:
          {
               std::any res_any = managers.at( ResourceType::Cubemap )->get_handler( params.resource.id );
               const auto *ptr = std::any_cast< Handler< ResourceType::Cubemap > >( &res_any );
               if ( !ptr )
               {
                    LOG_16NAR_ERROR( "Wrong handler for cubemap id " << params.resource.id );
                    return false;
               }
               descriptor = ptr->descriptor;
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
     // std::bad_alloc may be thrown here, but not later, so pushing data to vector will be safe
     color_attachments.reserve( params.attachments.size() );
     auto deleter = []( unsigned int *descriptor )
     {
          glBindFramebuffer( GL_FRAMEBUFFER, 0 );
          glDeleteFramebuffers( 1, descriptor );
     };
     glGenFramebuffers( 1, &handler.descriptor );
     glBindFramebuffer( GL_FRAMEBUFFER, handler.descriptor );
     std::unique_ptr< unsigned int, decltype( deleter ) > guard{ &handler.descriptor, deleter };

     for ( std::size_t i = 0; i < params.attachments.size(); i++ )
     {
          if ( !load_attachment( managers, params.attachments[ i ], handler, color_attachments ) )
          {
               return false;
          }
     }
     if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
     {
          LOG_16NAR_ERROR( "Unable to complete framebuffer" );
          return false;
     }
     guard.release();
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
