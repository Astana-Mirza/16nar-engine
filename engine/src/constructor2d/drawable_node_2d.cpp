#include <16nar/constructor2d/drawable_node_2d.h>

#include <16nar/constructor2d/system/scene_state.h>

namespace _16nar::constructor2d
{

DrawableNode2D::DrawableNode2D( const Shader& shader ) noexcept:
     Drawable2D::Drawable2D( shader )
{}


FloatRect DrawableNode2D::get_global_bounds() const
{
     return get_global_transform_matr() * get_local_bounds();
}


void DrawableNode2D::loop_call( SceneState& state, float delta, bool updated )
{
     if ( loop_func_ )
     {
          loop_func_( this, state, delta );
     }
     bool transformed = calculate_matr();
     updated = updated || transformed || updated_;
     updated_ = false;
     if ( updated )
     {
          set_render_system( &( state.get_render_system() ) ); // no effect if already set
          state.get_render_system().handle_change( this );
     }
     for ( auto& child : get_children() )
     {
          child->loop_call( state, delta, updated );
     }
}

} // namespace _16nar::constructor2d
