/// @file
/// Header file with SceneState class definition.
#ifndef _16NAR_SCENE_STATE_H
#define _16NAR_SCENE_STATE_H

#include <constructor/abstract/node.h>
#include <render/quadrant.h>

namespace _16nar
{

/// Simple state of scene which takes update and render flow.
class SceneState
{
public:
     /// Constructor sets updating and rendering of the state.
     /// @param updating set if this state will be updated in game loop.
     /// @param rendering set if this state will be rendered in game loop.
     SceneState( bool updating = true, bool rendering = true );

     /// Sets the updating option.
     /// @param rendering set if this state will be rendered in game loop.
     void set_rendering( bool rendering );

     /// Sets the updating option.
     /// @param updating set if this state will be updated in game loop.
     void set_updating( bool updating );

     /// Gets the updating option.
     bool get_updating() const;

     /// Gets the rendering option.
     bool get_rendering() const;

     /// Gets reference to the root quadrant.
     Quadrant& get_root_quadrant();

     /// Adds the node, it will have no parent.
     /// @param node pointer to node to be added.
     void add_node( Node *node );

     /// Removes the node, memory will not be freed.
     /// @param node pointer to node to be removed.
     void remove_node( Node *node );

     /// Function to be executed at scene startup.
     void setup();

     /// Function to be executed at scene update.
     /// @param delta time since last update, in seconds.
     void loop( float delta );

     /// Renders this state on the target.
     /// @param target target on which everething will be drawn.
     void render( RenderTarget& target );

protected:
     Quadrant root_;                ///< root quadrant of scene state.
     std::set< Node * > nodes_;     ///< set of this state's direct children nodes.

private:
     bool updating_;                ///< set if this state will be rendered in game loop.
     bool rendering_;               ///< set if this state will be rendered in game loop.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SCENE_STATE_H
