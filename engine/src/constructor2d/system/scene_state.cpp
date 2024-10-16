#include <16nar/constructor2d/system/scene_state.h>

namespace _16nar::constructor2d
{

SceneState::SceneState( std::unique_ptr< RenderSystem >&& render_system,
                        bool updating, bool rendering ):
     render_system_{ std::move( render_system ) },
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


void SceneState::start_render( RenderDevice& device )
{
     render_system_->start_render( view_, device );
}


void SceneState::finish_render( RenderDevice& device )
{
     render_system_->finish_render( device );
}

} // namespace _16nar::constructor2d
