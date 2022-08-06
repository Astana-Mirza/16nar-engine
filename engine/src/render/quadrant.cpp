#include <render/quadrant.h>

namespace _16nar
{

Quadrant::Quadrant( const FloatRect& area ):
     area_{ area }, children_{ nullptr, nullptr, nullptr, nullptr }, parent_{ nullptr }
{}

Quadrant::~Quadrant()
{
     for ( auto quad : children_ )
     {
          delete quad;
     }
}


const FloatRect& Quadrant::get_area() const
{
     return area_;
}


const Quadrant::QuadArray& Quadrant::get_children() const
{
     return children_;
}


Quadrant *Quadrant::get_parent() const
{
     return parent_;
}


void Quadrant::add_child( Quadrant* child, int index )
{
     child->parent_ = this;
     children_[ index ] = child;
}


void Quadrant::add_draw_child( Drawable *child )
{
     draw_layers_[ child->get_layer() ].insert( child );
}


void Quadrant::delete_draw_child( Drawable *child )
{
     auto& layer = draw_layers_[ child->get_layer() ];
     layer.erase( child );
     if ( layer.empty() )
     {
          draw_layers_.erase( child->get_layer() );
     }
}


void Quadrant::find_objects( RenderTarget& target, IntRect area, LayerMap& layers ) const
{
     IntRect mapped( target.mapCoordsToPixel( { area_.left, area_.top } ),
                     { ( int ) area_.width, ( int ) area_.height } );
     if ( mapped.intersects( area ) )
     {
          for ( const auto& pair : draw_layers_ )
          {
               layers.insert( pair );
          }
          for ( const auto ptr : children_ )
          {
               if ( ptr )
               {
                    ptr->find_objects( target, area, layers );
               }
          }
     }
}


void Quadrant::draw( RenderTarget& target, RenderStates& states ) const
{
     LayerMap layers;
     find_objects( target, target.getViewport( target.getView() ), layers );
     for ( const auto& [ lay, objs ] : layers )
     {
          for ( const auto ptr : objs )
          {
               if ( ptr->is_visible() )
               {
                    target.draw( *ptr, states );
               }
          }
     }
}

} // namespace _16nar
