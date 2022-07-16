#include <constructor/states/scene_state.h>

namespace _16nar
{

SceneState::SceneState( bool updating, bool rendering ):
     updating_{ updating }, rendering_{ rendering } {}


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


Quadrant& SceneState::get_root_quadrant()
{
     return root_;
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


void SceneState::render( RenderTarget& target )
{
     root_.draw( target, RenderStates::Default );
}

} // namespace _16nar
