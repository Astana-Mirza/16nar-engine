#include <16nar/render/opengl/st_render_device.h>

#include <16nar/render/opengl/shader.h>
#include <16nar/render/opengl/glad.h>

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
     Handler< ResourceType::FrameBuffer > frame_buffer_handler;
     const auto vertex_buffer_handler = std::any_cast< Handler< ResourceType::VertexBuffer > >(
          managers_.at( ResourceType::VertexBuffer )->get_handler( params.vertex_buffer.id ) );
     const auto shader_handler = std::any_cast< Handler< ResourceType::Shader > >(
          managers_.at( ResourceType::Shader )->get_handler( params.shader.id ) );

     if ( params.frame_buffer.id != 0 )
     {
          frame_buffer_handler = std::any_cast< Handler< ResourceType::FrameBuffer > >(
               managers_.at( ResourceType::FrameBuffer )->get_handler( params.frame_buffer.id ) );
     }
     glBindFramebuffer( GL_FRAMEBUFFER, frame_buffer_handler.descriptor ); // may be 0
     glBindVertexArray( vertex_buffer_handler.vao_descriptor );
     glUseProgram( shader_handler.descriptor );

     const auto& texture_manager = managers_.at( ResourceType::Texture );
     for ( std::size_t i = 0; i < params.textures.size(); i++ )
     {
          const auto texture_descriptor = std::any_cast< Handler< ResourceType::Texture > >(
               texture_manager->get_handler( params.textures[ i ].id ) );
          glActiveTexture( GL_TEXTURE0 + i );
          glBindTexture( GL_TEXTURE_2D, texture_descriptor.descriptor );
     }

     if ( params.setup )
     {
          opengl::Shader shader{ shader_handler.descriptor };
          params.setup( shader );
     }

     GLbitfield clear_mask = 0;
     clear_mask |= ( params.clear_color ? GL_COLOR_BUFFER_BIT : 0 );
     clear_mask |= ( params.clear_depth ? GL_DEPTH_BUFFER_BIT : 0 );
     clear_mask |= ( params.clear_stencil ? GL_STENCIL_BUFFER_BIT : 0 );
     glClear( clear_mask );

     if ( vertex_buffer_handler.ebo_descriptor != 0 )
     {
          glDrawElementsInstanced( primitive_type_to_int( params.primitive ),
               params.vertex_count, GL_UNSIGNED_INT, nullptr, params.instance_count );
     }
     else
     {
          glDrawArraysInstanced( primitive_type_to_int( params.primitive ),
               0, params.vertex_count, params.instance_count );
     }

     for ( std::size_t i = 0; i < params.textures.size(); i++ )
     {
          glActiveTexture( GL_TEXTURE0 + i );
          glBindTexture( GL_TEXTURE_2D, 0 );
     }
     glBindFramebuffer( GL_FRAMEBUFFER, 0 );
     glBindVertexArray( 0 );
     glUseProgram( 0 );
}

} // namespace _16nar::opengl
