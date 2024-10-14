#include <16nar/constructor2d/render/qtree_render_system.h>

#include <16nar/constructor2d/render/drawable_2d.h>

#include <16nar/game.h>
#include <16nar/render/irender_device.h>
#include <16nar/render/ishader_program.h>
#include <16nar/render/camera_2d.h>
#include <16nar/system/window.h>
#include <16nar/logger/logger.h>

#include <stdexcept>

namespace _16nar::constructor2d
{

void QTreeRenderSystem::reset()
{
     quad_map_.clear();
     layers_.clear();
     current_shader_ = 0;
     root_ = nullptr;
     camera_ = nullptr;
}


void QTreeRenderSystem::clear_screen()
{
     get_game().get_render_api().get_device()
          .clear( true, true, false );
}


void QTreeRenderSystem::select_objects()
{
     if ( !root_ )
     {
          LOG_16NAR_ERROR( "Root quadrant is not set for render system" );
          return;
     }
     if ( !camera_ )
     {
          LOG_16NAR_ERROR( "Camera is not set for render system" );
          return;
     }
     root_->find_objects( camera_->get_global_bounds(), layers_ );
     if ( layers_.empty() )
     {
          return;
     }
     for ( const auto& [ lay, objs ] : layers_ )
     {
          for ( const auto drawable : objs )
          {
               draw_object( drawable );
          }
     }
     layers_.clear();
}


void QTreeRenderSystem::draw_objects()
{
     get_game().get_render_api().process();
     get_game().get_render_api().end_frame();
     get_game().get_window().swap_buffers();
     current_shader_ = 0;
}


void QTreeRenderSystem::add_draw_child( Drawable2D *child )
{
     if ( !root_ )
     {
          LOG_16NAR_ERROR( "Root quadrant is not set for render system" );
          return;
     }
     quad_map_[ child ] = root_.get();
     root_->add_draw_child( child );
     handle_change( child );
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
     {
          // go to the lowest possible level
          found = false;
          const auto& children = current->get_children();
          for ( size_t i = 0; i < Quadrant::quad_count && !found; i++ )
          {
               if ( children[ i ] && check_quadrant( child, children[ i ].get() ) )
               {
                    current = children[ i ].get();
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


void QTreeRenderSystem::set_camera( Camera2D *camera )
{
     camera_ = camera;
}


const Camera2D* QTreeRenderSystem::get_camera() const
{
     return camera_;
}


void QTreeRenderSystem::set_root_quadrant( std::unique_ptr< Quadrant >&& root )
{
     root_ = std::move( root );
     quad_map_.clear();
}


Quadrant *QTreeRenderSystem::get_root_quadrant() const
{
     return root_.get();
}


bool QTreeRenderSystem::check_quadrant( const Drawable2D *obj, const Quadrant *quad )
{
     auto rect = obj->get_global_bounds();
     if ( quad->get_area().contains( rect.get_pos() ) &&
          quad->get_area().contains( rect.get_pos() + Vec2f{ rect.get_width(), rect.get_height() } ) )
     {
          return true;
     }
     return false;
}


void QTreeRenderSystem::draw_object( Drawable2D *obj )
{
     auto info = obj->get_draw_info();
     auto& device = get_game().get_render_api().get_device();
     if ( info.shader != current_shader_ )
     {
          current_shader_ = info.shader;
          device.bind_shader( info.shader );
          set_shader_params();
     }
     if ( info.shader_setup )
     {
          device.set_shader_params( info.shader_setup );
     }
     device.render( info.render_params );
}


void QTreeRenderSystem::set_shader_params() const
{
     if ( !camera_ )
     {
          LOG_16NAR_ERROR( "Camera is not set for render system" );
          return;
     }
     Vec2f size = camera_->get_size();
     TransformMatrix proj = TransformMatrix{}.scale( Vec2f{ 1.0f / size.x(), 1.0f / size.y() } );
     TransformMatrix view = camera_->get_transform_matr();
     get_game().get_render_api().get_device().set_shader_params(
          [ view, proj ]( const IShaderProgram& shader )
          {
               shader.set_uniform( "view_matr", view );
               shader.set_uniform( "proj_matr", proj );
          }
     );
}

} // namespace _16nar::constructor2d
