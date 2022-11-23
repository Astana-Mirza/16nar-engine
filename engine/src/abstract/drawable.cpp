#include <16nar/abstract/drawable.h>

namespace _16nar
{

Drawable::Drawable( Quadrant* quad ) : quad_{ quad }
{
     quad_->add_draw_child( this );
}


Drawable::~Drawable()
{
     quad_->delete_draw_child( this );
}


void Drawable::fix_quadrant()
{
     bool found = false;
     Quadrant* prev = quad_;
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

} // namespace _16nar
