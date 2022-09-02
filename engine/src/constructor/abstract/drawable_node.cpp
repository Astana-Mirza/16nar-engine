#include <constructor/abstract/drawable_node.h>

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
          fix_quadrant();
     }
     for ( auto child : get_children() )
     {
          child->loop_call( update, delta );
     }
}


bool DrawableNode::check_quadrant( const Quadrant *quad ) const
{
     auto transform = get_global_transform_matr();
     auto rect = transform.transformRect( get_local_bounds() );
     if ( quad->get_area().contains( rect.left, rect.top ) &&
          quad->get_area().contains( rect.left + rect.width - 1, rect.top + rect.height - 1 ) )
     {
          return true;
     }
     return false;
}

} // namespace _16nar
