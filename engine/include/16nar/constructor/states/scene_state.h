/// @file
/// @brief Header file with SceneState class definition.
#ifndef _16NAR_SCENE_STATE_H
#define _16NAR_SCENE_STATE_H

#include <16nar/constructor/abstract/node.h>
#include <16nar/abstract/render_system.h>

namespace _16nar
{

/// @brief Simple state of scene which takes update and render flow.
class ENGINE_API SceneState
{
public:
     /// @brief Constructor sets updating and rendering of the state.
     /// @param[in] render_system render system for the state.
     /// @param[in] view_rect rectangular area of a scene, which is displayed by a view.
     /// @param[in] updating set if this state will be updated in game loop.
     /// @param[in] rendering set if this state will be rendered in game loop.
     SceneState( std::unique_ptr< RenderSystem >&& render_system,
                 const FloatRect& view_rect,
                 bool updating = true, bool rendering = true );

     /// @brief Destructor deletes all nodes of this state.
     ~SceneState();

     /// @brief Sets the updating option.
     /// @param[in] rendering set if this state will be rendered in game loop.
     void set_rendering( bool rendering );

     /// @brief Sets the updating option.
     /// @param[in] updating set if this state will be updated in game loop.
     void set_updating( bool updating );

     /// @brief Gets the updating option.
     bool get_updating() const;

     /// @brief Gets the rendering option.
     bool get_rendering() const;

     /// @brief Gets view of the state.
     View& get_view();

     /// @brief Gets reference to the render system.
     RenderSystem& get_render_system();

     /// @brief Adds the node, it will have no parent.
     /// @param[in] node pointer to node to be added.
     void add_node( Node *node );

     /// @brief Removes the node, memory will not be freed.
     /// @param[in] node pointer to node to be removed.
     void remove_node( Node *node );

     /// @brief Function to be executed at scene startup.
     void setup();

     /// @brief Function to be executed at scene update.
     /// @param[in] delta time since last update, in seconds.
     void loop( float delta );

     /// @brief Start rendering this state on the target.
     /// @param[in] target target on which everething will be drawn.
     void start_render( RenderTarget& target );

     /// @brief Finish rendering this state.
     void finish_render();

protected:
     std::unique_ptr< RenderSystem > render_system_; ///< render system of this state.
     std::set< Node * > nodes_;                      ///< set of this state's direct children nodes.
     View view_;                                     ///< view of the state.

private:
     bool updating_;                ///< set if this state will be rendered in game loop.
     bool rendering_;               ///< set if this state will be rendered in game loop.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SCENE_STATE_H
