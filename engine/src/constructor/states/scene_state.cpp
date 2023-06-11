#include <16nar/constructor/states/scene_state.h>

namespace _16nar
{

SceneState::SceneState( std::unique_ptr< RenderSystem >&& render_system,
                        const FloatRect& view_rect,
                        bool updating, bool rendering ):
     render_system_{ std::move( render_system ) }, view_{ view_rect },
     updating_{ updating }, rendering_{ rendering } {}


SceneState::~SceneState()
{
     for ( Node* node : nodes_ )
     {
          delete node;
     }
}


void SceneState::set_rendering( bool rendering )
{
     rendering_ = rendering;
}


void SceneState::set_updating( bool updating )
{
     updating_ = updating;
}


bool SceneState::get_updating() const
{
     return updating_;
}


bool SceneState::get_rendering() const
{
     return rendering_;
}


View& SceneState::get_view()
{
     return view_;
}


RenderSystem& SceneState::get_render_system()
{
     if ( !render_system_ )
     {
          throw std::runtime_error{ "render system not set" };
     }
     return *render_system_;
}


void SceneState::add_node( Node *node )
{
     nodes_.insert( node );
}


void SceneState::remove_node( Node *node )
{
     nodes_.erase( node );
}


void SceneState::setup()
{
     for ( Node *node : nodes_ )
     {
          node->setup_call();
     }
}


void SceneState::loop( float delta )
{
     for ( Node *node : nodes_ )
     {
          node->loop_call( false, delta );
     }
}


void SceneState::start_render( RenderTarget& target )
{
     target.setView( view_ );
     render_system_->start_render( target );
}


void SceneState::finish_render()
{
     render_system_->finish_render();
}

} // namespace _16nar
