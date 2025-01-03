#include <16nar/constructor2d/system/scene_state.h>

#include <cassert>

namespace _16nar::constructor2d
{

SceneState::SceneState( std::unique_ptr< IRenderSystem2D >&& render_system,
                        bool updating, bool rendering ):
     render_system_{ std::move( render_system ) }, nodes_{},
     updating_{ updating }, rendering_{ rendering } {}


void SceneState::set_rendering( bool rendering ) noexcept
{
     rendering_ = rendering;
}


void SceneState::set_updating( bool updating ) noexcept
{
     updating_ = updating;
}


bool SceneState::get_updating() const noexcept
{
     return updating_;
}


bool SceneState::get_rendering() const noexcept
{
     return rendering_;
}


IRenderSystem2D& SceneState::get_render_system()
{
     assert( render_system_ );
     return *render_system_;
}


void SceneState::setup()
{
     for ( auto& node : nodes_ )
     {
          node->setup_call( *this );
     }
}


void SceneState::loop( float delta )
{
     for ( auto& node : nodes_ )
     {
          node->loop_call( *this, delta, false );
     }
}


void SceneState::add_node( std::unique_ptr< Node2D >&& node )
{
     auto pair = nodes_.insert( std::move( node ) );
}


std::unique_ptr< Node2D > SceneState::remove_node( Node2D *node )
{
     auto iter = nodes_.find( node );
     if ( iter != nodes_.end() )
     {
          auto handle = nodes_.extract( iter );
          auto ptr{ std::move( handle.value() ) };
          return ptr;
     }
     return std::unique_ptr< Node2D >{};
}

} // namespace _16nar::constructor2d
