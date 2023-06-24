#include <16nar/render/qtree_render_system.h>
#include <16nar/abstract/render_device.h>
#include <16nar/render/view.h>

namespace _16nar
{

QTreeRenderSystem::QTreeRenderSystem( Quadrant&& root ): root_{ root } {}


void QTreeRenderSystem::start_render( const View& view, RenderDevice& device )
{
     Quadrant::LayerMap layers;
     root_.find_objects( view.get_global_bounds(), layers );
     if ( !layers.empty() )
     {
          RenderDevice::DeviceData data{};
          data.type = RenderDevice::DeviceDataType::View;
          data.view = view;
          device.add_data( data );
     }
     for ( const auto& [ lay, objs ] : layers )
     {
          for ( const auto drawable : objs )
          {
               if ( drawable->is_visible() )
               {
                    RenderDevice::DeviceData data{};
                    data.type = RenderDevice::DeviceDataType::View;
                    data.render_data = drawable->get_render_data();
                    device.add_data( data );
               }
          }
     }
}


void QTreeRenderSystem::finish_render( RenderDevice& device )
{
     device.flush();
}


void QTreeRenderSystem::add_draw_child( Drawable *child )
{
     quad_map_[ child ] = &root_;
     root_.add_draw_child( child );
}


void QTreeRenderSystem::delete_draw_child( Drawable *child )
{
     auto iter = quad_map_.find( child );
     if ( iter != quad_map_.cend() )
     {
          iter->second->delete_draw_child( child );
          quad_map_.erase( iter );
     }
}


void QTreeRenderSystem::handle_change( Drawable *child )
{
     bool found = false;
     auto iter = quad_map_.find( child );
     if ( iter == quad_map_.cend() )
     {
          return;
     }
     Quadrant *prev = iter->second;
     Quadrant *current = prev;
     while ( current->get_parent() && !check_quadrant( child, current ) )
     {
          current = current->get_parent();
     }
     do
     {    // go to the lowest possible level
          const auto& children = current->get_children();
          for ( size_t i = 0; i < Quadrant::quad_count && !found; i++ )
          {
               if ( children[ i ] && check_quadrant( child, children[ i ] ) )
               {
                    current = children[ i ];
                    found = true;
               }
          }
     }
     while ( found );
     if ( prev != current )
     {
          prev->delete_draw_child( child );
          current->add_draw_child( child );
          quad_map_[ child ] = current;
     }
}


Quadrant& QTreeRenderSystem::get_root()
{
     return root_;
}


bool QTreeRenderSystem::check_quadrant( Drawable *obj, const Quadrant *quad )
{
     auto rect = obj->get_global_bounds();
     if ( quad->get_area().contains( rect.left, rect.top ) &&
          quad->get_area().contains( rect.left + rect.width - 1, rect.top + rect.height - 1 ) )
     {
          return true;
     }
     return false;
}


} // namespace _16nar
