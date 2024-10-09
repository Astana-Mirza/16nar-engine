/// @file
/// @brief Header file with SceneState class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_SCENE_STATE_H
#define _16NAR_CONSTRUCTOR_2D_SCENE_STATE_H

#include <16nar/constructor2d/render/irender_system_2d.h>
#include <16nar/render/view.h>

namespace _16nar::constructor2d
{

class Node2D;

/// @brief Simple state of scene which takes update and render flow.
class ENGINE_API SceneState
{
public:
     /// @brief Constructor, sets updating and rendering of the state.
     /// @param[in] render_system render system for the state.
     /// @param[in] updating set if this state will be updated in game loop.
     /// @param[in] rendering set if this state will be rendered in game loop.
     SceneState( std::unique_ptr< IRenderSystem2D >&& render_system,
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
     IRenderSystem2D& get_render_system();

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
     /// @param[in] device device with which everething will be rendered.
     void start_render( RenderDevice& device );

     /// @brief Finish rendering this state.
     /// @param[in] device device with which everething will be rendered.
     void finish_render( RenderDevice& device );

protected:
     std::unique_ptr< IRenderSystem > render_system_;  ///< render system of this state.
     std::unordered_set< Node * > nodes_;              ///< set of this state's direct children nodes.

private:
     bool updating_;                ///< set if this state will be rendered in game loop.
     bool rendering_;               ///< set if this state will be rendered in game loop.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_SCENE_STATE_H
