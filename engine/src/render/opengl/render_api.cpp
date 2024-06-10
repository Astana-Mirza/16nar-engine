#include <16nar/render/opengl/render_api.h>

#include <16nar/render/opengl/st_resource_manager.h>
#include <16nar/render/opengl/mt_resource_manager.h>
#include <16nar/render/opengl/st_render_device.h>
#include <16nar/render/opengl/mt_render_device.h>
#include <16nar/render/opengl/frame_buffer_loader.h>
#include <16nar/render/opengl/vertex_buffer_loader.h>
#include <16nar/render/opengl/render_buffer_loader.h>
#include <16nar/render/opengl/texture_loader.h>
#include <16nar/render/opengl/shader_loader.h>
#include <16nar/render/opengl/cubemap_loader.h>
#include <16nar/render/opengl/glad.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

#include <GLFW/glfw3.h>

namespace _16nar::opengl
{

RenderApi::RenderApi( ProfileType profile )
{
     if ( !gladLoadGLLoader( reinterpret_cast< GLADloadproc >( glfwGetProcAddress ) ) )
     {
          throw std::runtime_error{ "cannot create OpenGL render API, failed to initialize GLAD" };
     }

     switch ( profile )
     {
          case ProfileType::SingleThreaded:
          {
               managers_.emplace( ResourceType::FrameBuffer,
                    std::make_unique< StResourceManager< FrameBufferLoader > >( managers_ ) );
               managers_.emplace( ResourceType::VertexBuffer,
                    std::make_unique< StResourceManager< VertexBufferLoader > >( managers_ ) );
               managers_.emplace( ResourceType::Texture,
                    std::make_unique< StResourceManager< TextureLoader > >( managers_ ) );
               managers_.emplace( ResourceType::Shader,
                    std::make_unique< StResourceManager< ShaderLoader > >( managers_ ) );
               managers_.emplace( ResourceType::RenderBuffer,
                    std::make_unique< StResourceManager< RenderBufferLoader > >( managers_ ) );
               managers_.emplace( ResourceType::Cubemap,
                    std::make_unique< StResourceManager< CubemapLoader > >( managers_ ) );

               device_ = std::make_unique< StRenderDevice >( managers_ );
          }
          break;
          case ProfileType::MultiThreaded:
          {
               managers_.emplace( ResourceType::FrameBuffer,
                    std::make_unique< MtResourceManager< FrameBufferLoader > >( managers_ ) );
               managers_.emplace( ResourceType::VertexBuffer,
                    std::make_unique< MtResourceManager< VertexBufferLoader > >( managers_ ) );
               managers_.emplace( ResourceType::Texture,
                    std::make_unique< MtResourceManager< TextureLoader > >( managers_ ) );
               managers_.emplace( ResourceType::Shader,
                    std::make_unique< MtResourceManager< ShaderLoader > >( managers_ ) );
               managers_.emplace( ResourceType::RenderBuffer,
                    std::make_unique< MtResourceManager< RenderBufferLoader > >( managers_ ) );
               managers_.emplace( ResourceType::Cubemap,
                    std::make_unique< MtResourceManager< CubemapLoader > >( managers_ ) );

               device_ = std::make_unique< MtRenderDevice >( managers_ );
          }
          break;
          default:
               LOG_16NAR_ERROR( "Cannot create OpenGL render API, wrong profile" );
               throw std::runtime_error{ "cannot create OpenGL render API, wrong profile" };
          break;
     }
}


Resource RenderApi::load( ResourceType type, const std::any& params )
{
     const auto iter = managers_.find( type );
     if ( iter == managers_.cend() )
     {
          throw ResourceException{ "wrong resource type" };
     }
     return Resource{ type, iter->second->load( params ) };
}


void RenderApi::unload( const Resource& resource )
{
     const auto iter = managers_.find( resource.type );
     if ( iter == managers_.cend() )
     {
          throw ResourceException{ "wrong resource type" };
     }
     iter->second->unload( resource.id );
}


IRenderDevice& RenderApi::get_device() const
{
     return *device_;
}


void RenderApi::process()
{
     for ( auto& pair : managers_ )
     {
          if ( pair.first != ResourceType::FrameBuffer )
          {
               pair.second->process_load_queue();
          }
     }
     // process frame buffer after attachments are loaded in other managers.
     managers_[ ResourceType::FrameBuffer ]->process_load_queue();

     device_->process_render_queue();
     for ( auto& pair : managers_ )
     {
          pair.second->process_unload_queue();
     }
}


void RenderApi::end_frame()
{
     for ( auto& pair : managers_ )
     {
          pair.second->end_frame();
     }
     device_->end_frame();
}

} // namespace _16nar::opengl
