#include <constructor/states/scene_state.h>

namespace _16nar
{

SceneState::SceneState( const FloatRect& view_rect, bool updating, bool rendering ):
     view_{ view_rect }, updating_{ updating }, rendering_{ rendering } {}


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
     target.setView( view_ );
     root_.draw( target );
}

} // namespace _16nar
