#include <constructor/world_node.h>

namespace _16nar
{

void WorldNode::setup()
{
     for ( auto& [ order, state ] : states_ )
     {
          state->setup();
     }
}


void WorldNode::loop( float delta )
{
     for ( auto& [ order, state ] : states_ )
     {
          if ( state->get_updating() )
          {
               state->loop( delta );
          }
     }
}


void WorldNode::render( RenderTarget& target )
{
     for ( auto& [ order, state ] : states_ )
     {
          if ( state->get_rendering() )
          {
               state->render( target );
          }
     }
}


void WorldNode::register_state( int order, std::unique_ptr< SceneState >&& state )
{
     states_[ order ] = std::move( state );
}


} // namespace _16nar
