#include <16nar/render/opengl/st_render_device.h>

#include <16nar/render/opengl/shader_program.h>
#include <16nar/render/opengl/glad.h>
#include <16nar/system/exceptions.h>

namespace _16nar::opengl
{

StRenderDevice::StRenderDevice( const ResourceManagerMap& managers ):
     managers_{ managers }, current_shader_{}
{}


void StRenderDevice::render( const RenderParams& params )
{
     // managers are handled by render API, so at() must never throw here
     const auto& texture_manager = managers_.at( ResourceType::Texture );
     std::vector< Handler< ResourceType::Texture > > handlers;
     handlers.reserve( params.textures.size() );
     for ( const auto& texture : params.textures )
     {
          std::any tex_param_any = texture_manager->get_handler( texture.id );
          const auto *tex_param_ptr = std::any_cast< Handler< ResourceType::Texture > >( &tex_param_any );
          if ( !tex_param_ptr )
          {
               throw ResourceException{ "wrong handler for texture id ", texture.id };
          }
          handlers.emplace_back( *tex_param_ptr );
     }

     std::any vb_any =
          managers_.at( ResourceType::VertexBuffer )->get_handler( params.vertex_buffer.id );
     const auto *vb_ptr = std::any_cast< Handler< ResourceType::VertexBuffer > >( &vb_any );
     if ( !vb_ptr )
     {
          throw ResourceException{ "wrong handler for vertex buffer id ", params.vertex_buffer.id };
     }

     for ( std::size_t i = 0; i < handlers.size(); i++ )
     {
          glActiveTexture( GL_TEXTURE0 + i );
          glBindTexture( GL_TEXTURE_2D, handlers[ i ].descriptor );
     }

     glBindVertexArray( vb_ptr->vao_descriptor );

     if ( vb_ptr->ebo_descriptor != 0 )
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

     // managers are handled by render API, so at() must never throw here
     std::any shader_handler_any = managers_.at( ResourceType::Shader )->get_handler( shader.id );
     const auto *shader_handler = std::any_cast< Handler< ResourceType::Shader > >( &shader_handler_any );
     if ( !shader_handler )
     {
          throw ResourceException{ "wrong handler for shader id ", shader.id };
     }

     current_shader_ = *shader_handler;
     glUseProgram( shader_handler->descriptor );
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
     // managers are handled by render API, so at() must never throw here
     std::any fb_any = managers_.at( ResourceType::FrameBuffer )->get_handler( framebuffer.id );
     const auto *fb_ptr = std::any_cast< Handler< ResourceType::FrameBuffer > >( &fb_any );
     if ( !fb_ptr )
     {
          throw ResourceException{ "wrong handler for framebuffer id ", framebuffer.id };
     }
     glBindFramebuffer( GL_FRAMEBUFFER, fb_ptr->descriptor );
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
