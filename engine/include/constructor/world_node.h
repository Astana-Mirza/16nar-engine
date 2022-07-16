/// @file
/// Header file with WorldNode class definition.
#ifndef _16NAR_WORLD_NODE_H
#define _16NAR_WORLD_NODE_H

#include <constructor/states/scene_state.h>
#include <map>
#include <memory>

namespace _16nar
{

/// The world, root of the scene tree. The world is a finite state machine,
/// game states (e.g. main game, pause, mini-game inside main game) can have
/// their own rengering and physics handling.
class WorldNode
{
public:
     /// Setup function called once.
     void setup();

     /// Loop function called in game loop.
     /// @parm delta time since previous loop call, in seconds.
     void loop( float delta );

     /// Renders all states on the target.
     /// @param target target on which everething will be drawn.
     void render( RenderTarget& target );

     /// Register new simple scene state.
     /// @param order order value which defines order of state updating.
     /// @param state pointer to scene state.
     void register_state( int order, std::unique_ptr< SceneState >&& state );

private:
     std::map< int, std::unique_ptr< SceneState > > states_;     ///< states of this scene with their order.
};

} // namespace _16nar

#endif // #ifndef _16NAR_WORLD_NODE_H
