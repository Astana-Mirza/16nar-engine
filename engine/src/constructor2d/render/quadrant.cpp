#include <16nar/constructor2d/render/quadrant.h>

#include <16nar/constructor2d/render/drawable_2d.h>

namespace _16nar::constructor2d
{

Quadrant::Quadrant( const FloatRect& area ):
     drawables_{}, area_{ area }, children_{}, parent_{ nullptr }
{}


const FloatRect& Quadrant::get_area() const
{
     return area_;
}


const Quadrant::QuadArray& Quadrant::get_children() const
{
     return children_;
}


const Quadrant::DrawableSet& Quadrant::get_draw_children() const
{
     return drawables_;
}


Quadrant *Quadrant::get_parent() const
{
     return parent_;
}


void Quadrant::add_child( std::unique_ptr< Quadrant >&& child, int index )
{
     child->parent_ = this;
     children_[ index ] = std::move( child );
}


void Quadrant::add_draw_child( Drawable2D *child )
{
     drawables_.insert( child );
}


void Quadrant::delete_draw_child( Drawable2D *child )
{
     drawables_.erase( child );
}


void Quadrant::find_objects( const FloatRect& area, LayerMap& layers ) const
{
     if ( area_.intersects( area ) )
     {
          for ( const auto ptr : drawables_ )
          {
               if ( ptr->is_visible() )
               {
                    layers[ ptr->get_layer() ].insert( ptr );
               }
          }
          for ( const auto& ptr : children_ )
          {
               if ( ptr )
               {
                    ptr->find_objects( area, layers );
               }
          }
     }
}

} // namespace _16nar::constructor2d
