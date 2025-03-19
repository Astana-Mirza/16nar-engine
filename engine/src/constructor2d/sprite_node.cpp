#include <16nar/constructor2d/sprite_node.h>

#include <16nar/game.h>
#include <16nar/render/irender_api.h>
#include <16nar/render/ishader_program.h>

namespace _16nar::constructor2d
{

SpriteNode::SpriteNode( const Shader& shader, const TextureRect& texture_rect, const FloatRect& local_bounds ):
     DrawableNode2D::DrawableNode2D( shader ), texture_rect_{ texture_rect }, local_bounds_{ local_bounds }
{
     const float tex_x1 = texture_rect_.rectangle.get_pos().x();
     const float tex_y1 = texture_rect_.rectangle.get_pos().y();
     const float tex_x2 = texture_rect_.rectangle.get_end().x();
     const float tex_y2 = texture_rect_.rectangle.get_end().y();

     const float vertices[] = {
          local_bounds_.get_width(), local_bounds_.get_height(), tex_x2, tex_y2,
          local_bounds_.get_width(), 0.0f, tex_x2, tex_y1,
          0.0f, 0.0f, tex_x1, tex_y1,
          0.0f, local_bounds_.get_height(), tex_x1, tex_y2
     };

     LoadParams< ResourceType::VertexBuffer > vb_load_params;
     vb_load_params.buffer.data = DataSharedPtr{
          new std::byte[ sizeof( vertices ) ], std::default_delete< std::byte[] >() };
     std::memcpy( reinterpret_cast< char * >( vb_load_params.buffer.data.get() ), vertices, sizeof( vertices ) );
     vb_load_params.buffer.size = sizeof( vertices );
     vb_load_params.buffer.type = BufferType::StaticDraw;
     vb_load_params.attributes.push_back( { 2, DataType::Float, false } );
     vb_load_params.attributes.push_back( { 2, DataType::Float, false } );

     ResID vb_id = get_game().get_render_api().load( ResourceType::VertexBuffer, vb_load_params ).id;
     vertex_buffer_ = VertexBuffer( vb_id );
}


DrawInfo SpriteNode::get_draw_info() const
{
     DrawInfo info{};
     info.render_params.textures.push_back( texture_rect_.texture );
     info.render_params.vertex_buffer = vertex_buffer_;
     info.render_params.primitive = PrimitiveType::TriangleStrip;
     info.render_params.vertex_count = 4;
     info.render_params.instance_count = 1;

     auto model_matr = get_global_transform_matr();
     info.shader_setup = [ model_matr ]( const IShaderProgram& shader )
     {
          shader.set_uniform( "model_matr", model_matr );
     };

     info.shader = shader_;
     return info;
}


FloatRect SpriteNode::get_local_bounds() const
{
     return local_bounds_;
}

} // namespace _16nar::constructor2d
