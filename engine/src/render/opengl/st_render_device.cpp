#include <16nar/render/opengl/st_render_device.h>

#include <16nar/render/opengl/shader_program.h>
#include <16nar/render/opengl/glad.h>

namespace _16nar::opengl
{

StRenderDevice::StRenderDevice( const ResourceManagerMap& managers ):
     managers_{ managers }, current_shader_{}
{}


void StRenderDevice::render( const RenderParams& params )
{
     const auto& texture_manager = managers_.at( ResourceType::Texture );
     for ( std::size_t i = 0; i < params.textures.size(); i++ )
     {
          const auto texture_descriptor = std::any_cast< Handler< ResourceType::Texture > >(
               texture_manager->get_handler( params.textures[ i ].id ) );
          glActiveTexture( GL_TEXTURE0 + i );
          glBindTexture( GL_TEXTURE_2D, texture_descriptor.descriptor );
     }

     const auto vertex_buffer_handler = std::any_cast< Handler< ResourceType::VertexBuffer > >(
          managers_.at( ResourceType::VertexBuffer )->get_handler( params.vertex_buffer.id ) );

     glBindVertexArray( vertex_buffer_handler.vao_descriptor );

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
}


void StRenderDevice::set_viewport( const IntRect& rect )
{
     glViewport( rect.get_pos().x(), rect.get_pos().y(), rect.get_width(), rect.get_height() );
}


void StRenderDevice::set_depth_test_state( bool enable )
{
     if ( enable )
     {
          glEnable( GL_DEPTH_TEST );
     }
     else
     {
          glDisable( GL_DEPTH_TEST );
     }
}


void StRenderDevice::bind_shader( const Shader& shader )
{
     if ( shader.id == 0 )
     {
          glUseProgram( 0 );
          current_shader_ = {};
          return;
     }

     const auto shader_handler = std::any_cast< Handler< ResourceType::Shader > >(
          managers_.at( ResourceType::Shader )->get_handler( shader.id ) );

     current_shader_ = shader_handler;
     glUseProgram( shader_handler.descriptor );
}


void StRenderDevice::set_shader_params( const std::function< void( const IShaderProgram& ) >& setup )
{
     if ( current_shader_.descriptor == 0 )
     {
          return;
     }
     ShaderProgram shader{ current_shader_.descriptor };
     setup( shader );
}


void StRenderDevice::bind_framebuffer( const FrameBuffer& framebuffer )
{
     if ( framebuffer.id == 0 )
     {
          glBindFramebuffer( GL_FRAMEBUFFER, 0 );
          return;
     }
     const auto frame_buffer_handler = std::any_cast< Handler< ResourceType::FrameBuffer > >(
          managers_.at( ResourceType::FrameBuffer )->get_handler( framebuffer.id ) );
     glBindFramebuffer( GL_FRAMEBUFFER, frame_buffer_handler.descriptor );
}


void StRenderDevice::clear( bool color, bool depth, bool stencil )
{
     GLbitfield clear_mask = 0;
     clear_mask |= ( color ? GL_COLOR_BUFFER_BIT : 0 );
     clear_mask |= ( depth ? GL_DEPTH_BUFFER_BIT : 0 );
     clear_mask |= ( stencil ? GL_STENCIL_BUFFER_BIT : 0 );
     if ( clear_mask )
     {
          glClear( clear_mask );
     }
}

} // namespace _16nar::opengl
