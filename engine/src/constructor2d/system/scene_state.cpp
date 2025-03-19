#include <16nar/constructor2d/system/scene_state.h>

#include <stdexcept>
#include <cassert>

namespace _16nar::constructor2d
{

SceneState::SceneState( int order, std::unique_ptr< IRenderSystem2D >&& render_system,
                        bool updating, bool rendering ):
     render_system_{ std::move( render_system ) }, nodes_{}, order_{ order },
     updating_{ updating }, rendering_{ rendering } {}


int SceneState::get_order() const noexcept
{
     return order_;
}


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


void SceneState::add_child( std::unique_ptr< Node2D >&& node )
{
     auto& ptr = nodes_.emplace_back( std::move( node ) );
     ptr->parent_ = nullptr;
     ptr->set_state_order( order_ );
     ptr->updated_ = true;
}


std::unique_ptr< Node2D > SceneState::remove_child( const Node2D *node )
{
     auto iter = std::find_if( nodes_.begin(), nodes_.end(),
          [ node ]( const auto& ptr ){ return ptr.get() == node; } );
     if ( iter != nodes_.end() )
     {
          auto ptr{ std::move( *iter ) };
          nodes_.erase( iter );
          return ptr;
     }
     return std::unique_ptr< Node2D >{};
}

} // namespace _16nar::constructor2d
