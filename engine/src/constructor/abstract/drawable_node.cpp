#include <constructor/abstract/drawable_node.h>

namespace _16nar
{

DrawableNode::DrawableNode( Quadrant *quad ) : quad_{ quad }
{
     quad_->add_draw_child( this );
}


DrawableNode::~DrawableNode()
{
     quad_->delete_draw_child( this );
}


void DrawableNode::fix_quadrant()
{
     bool found = false;
     Quadrant *prev = quad_;
     while ( quad_->get_parent() && !check_quadrant( quad_ ) )
     {
          quad_ = quad_->get_parent();
     }
     do
     {                        // go to the lowest possible level
          found = false;
          const auto& children = quad_->get_children();
          for ( size_t i = 0; i < Quadrant::quad_count && !found; i++ )
          {
               if ( children[ i ] && check_quadrant( children[ i ] ) )
               {
                    quad_ = children[ i ];
                    found = true;
               }
          }
     }
     while ( found );
     if ( prev != quad_ )
     {
          prev->delete_draw_child( this );
          quad_->add_draw_child( this );
     }
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


bool DrawableNode::check_quadrant( const Quadrant* quad ) const
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
