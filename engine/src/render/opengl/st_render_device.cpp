#include <16nar/render/opengl/st_render_device.h>

namespace _16nar::opengl
{

StRenderDevice::StRenderDevice( const ResourceManagerMap& managers ):
     managers_{ managers }
{}


void StRenderDevice::render( const RenderParams& params )
{
     do_render( params );
}


void StRenderDevice::do_render( const RenderParams& params )
{
     const auto frame_buffer_handler = std::any_cast< Handler< ResourceType::FrameBuffer > >(
          managers_.at( ResourceType::FrameBuffer )->get_handler( params.frame_buffer.id ) );
     const auto vertex_buffer_handler = std::any_cast< Handler< ResourceType::VertexBuffer > >(
          managers_.at( ResourceType::VertexBuffer )->get_handler( params.vertex_buffer.id ) );
     const auto shader_handler = std::any_cast< Handler< ResourceType::Shader > >(
          managers_.at( ResourceType::Shader )->get_handler( params.frame_buffer.id ) );

     // bind framebuffer
     // bind VAO
     // bind shader

     const auto& texture_manager = managers_.at( ResourceType::Texture );
     for ( std::size_t i = 0; i < params.textures.size(); i++ )
     {
          const auto texture_descriptor = std::any_cast< Handler< ResourceType::Texture > >(
               texture_manager->get_handler( params.textures[ i ].id) );
          // bind i-th texture
     }
     // Shader shader{ shader_handler.descriptor };
     // params.setup( shader );
     // draw call
}

} // namespace _16nar::opengl
