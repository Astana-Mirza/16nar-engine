/// @file
/// @brief Header file with SceneState class definition.
#ifndef _16NAR_SCENE_STATE_H
#define _16NAR_SCENE_STATE_H

#include <16nar/constructor/abstract/node.h>
#include <16nar/render/quadrant.h>

namespace _16nar
{

/// @brief Simple state of scene which takes update and render flow.
class ENGINE_API SceneState
{
public:
     /// @brief Constructor sets updating and rendering of the state.
     /// @param view_rect rectangular area of a scene, which is displayed by a view.
     /// @param updating set if this state will be updated in game loop.
     /// @param rendering set if this state will be rendered in game loop.
     SceneState( const FloatRect& view_rect, bool updating = true, bool rendering = true );

     /// @brief Destructor deletes all nodes of this state.
     ~SceneState();

     /// @brief Sets the updating option.
     /// @param rendering set if this state will be rendered in game loop.
     void set_rendering( bool rendering );

     /// @brief Sets the updating option.
     /// @param updating set if this state will be updated in game loop.
     void set_updating( bool updating );

     /// @brief Gets the updating option.
     bool get_updating() const;

     /// @brief Gets the rendering option.
     bool get_rendering() const;

     /// @brief Gets view of the state.
     View& get_view();

     /// @brief Gets reference to the root quadrant.
     Quadrant& get_root_quadrant();

     /// @brief Adds the node, it will have no parent.
     /// @param node pointer to node to be added.
     void add_node( Node *node );

     /// @brief Removes the node, memory will not be freed.
     /// @param node pointer to node to be removed.
     void remove_node( Node *node );

     /// @brief Function to be executed at scene startup.
     void setup();

     /// @brief Function to be executed at scene update.
     /// @param delta time since last update, in seconds.
     void loop( float delta );

     /// @brief Renders this state on the target.
     /// @param target target on which everething will be drawn.
     void render( RenderTarget& target );

protected:
     Quadrant root_;                ///< root quadrant of scene state.
     std::set< Node * > nodes_;     ///< set of this state's direct children nodes.
     View view_;                    ///< view of the state.

private:
     bool updating_;                ///< set if this state will be rendered in game loop.
     bool rendering_;               ///< set if this state will be rendered in game loop.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SCENE_STATE_H
