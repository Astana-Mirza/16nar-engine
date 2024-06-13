#include <16nar/constructor/render/qtree_render_system.h>

#include <16nar/constructor/render/drawable_2d.h>
#include <16nar/logger/logger.h>

namespace _16nar::constructor
{

QTreeRenderSystem::QTreeRenderSystem( std::unique_ptr< IRenderApi >&& api, Quadrant&& root ):
     quad_map_{}, root_{ root }, api_{ std::move( api ) }, current_shader_{}
{}


void QTreeRenderSystem::select_objects()
{
     /*Quadrant::LayerMap layers;
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
     }*/
}


void QTreeRenderSystem::draw_objects()
{

}


void QTreeRenderSystem::add_draw_child( Drawable2D *child )
{
     quad_map_[ child ] = &root_;
     root_.add_draw_child( child );
}


void QTreeRenderSystem::delete_draw_child( Drawable2D *child )
{
     auto iter = quad_map_.find( child );
     if ( iter != quad_map_.cend() )
     {
          iter->second->delete_draw_child( child );
          quad_map_.erase( iter );
     }
}


void QTreeRenderSystem::handle_change( Drawable2D *child )
{
     bool found = false;
     auto iter = quad_map_.find( child );
     if ( iter == quad_map_.cend() )
     {
          LOG_16NAR_ERROR( "No such node in current render system" );
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


bool QTreeRenderSystem::check_quadrant( Drawable2D *obj, const Quadrant *quad )
{
     auto rect = obj->get_global_bounds();
     if ( quad->get_area().contains( rect.get_pos() ) &&
          quad->get_area().contains( rect.get_pos() + Vec2f{ rect.get_width(), rect.get_height() } ) )
     {
          return true;
     }
     return false;
}


} // namespace _16nar::constructor
