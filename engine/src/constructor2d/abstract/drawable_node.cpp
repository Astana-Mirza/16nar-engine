#include <16nar/constructor/abstract/drawable_node.h>

namespace _16nar
{

bool DrawableNode::is_visible() const
{
     return visible_;
}


void DrawableNode::set_visible( bool visible )
{
     visible_ = visible;
}


int DrawableNode::get_layer() const
{
     return layer_;
}


void DrawableNode::set_layer( int layer )
{
     layer_ = layer;
}


void DrawableNode::set_shader( Shader *shader )
{
     shader_ = shader;
}


Shader *DrawableNode::get_shader() const
{
     return shader_;
}


void DrawableNode::set_blend( const BlendMode& blend )
{
     blend_ = blend;
}


const BlendMode& DrawableNode::get_blend() const
{
     return blend_;
}


FloatRect DrawableNode::get_global_bounds() const
{
     auto transform = get_global_transform_matr();
     return transform.transformRect( get_local_bounds() );
}


void DrawableNode::loop_call( bool update, float delta )
{
     loop( delta );
     for ( auto& [name, anim] : animations_ )
     {
          anim->advance( delta );
     }
     update = update || transformed_;
     transformed_ = false;
     if ( update )
     {
          render_system_->handle_change( this );
     }
     for ( auto child : get_children() )
     {
          child->loop_call( update, delta );
     }
}

} // namespace _16nar
