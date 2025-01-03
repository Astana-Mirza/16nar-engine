/// @file
/// @brief Header file with Scene class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_SCENE_H
#define _16NAR_CONSTRUCTOR_2D_SCENE_H

#include <16nar/16nardefs.h>

#include <map>
#include <memory>

namespace _16nar::constructor2d
{

class SceneState;
class Node2D;

/// @brief Root object of the scene tree.
/// @details Scene consists of scene states, each of which can be rendered and updated
/// individually. States are handled sequentially in strict order.
class ENGINE_API Scene
{
public:
     Scene( const Scene& )            = delete;
     Scene& operator=( const Scene& ) = delete;

     using SetupFuncPtr = void ( * )();
     using LoopFuncPtr = void ( * )( float );

     /// @brief Default constructor.
     Scene();

     /// @brief Swap scene content with another scene.
     /// @param[in] other scene to exchange content.
     void swap( Scene& other );

     /// @brief Call scene's saved setup function, if it exists, and setup for all states.
     /// @details Setup function should be called once at scene startup.
     void setup();

     /// @brief Call scene's saved loop function, if it exists, and loop for all states.
     /// @param[in] delta time since previous loop call, in seconds.
     void loop( float delta );

     /// @brief Register new simple scene state.
     /// @param[in] order order value which defines order of state updating.
     /// @param[in] state pointer to scene state.
     void register_state( int order, SceneState&& state );

     /// @brief Get the scene state with given order, throws exception if no such state exists.
     /// @param[in] order order of the state.
     /// @return scene state.
     SceneState& get_state( int order );

     /// @copydoc SceneState::get_state(int)
     const SceneState& get_state( int order ) const;

     /// @brief Change parent of the node.
     /// @throws std::runtime_error if @b node has no parent and state saved in @b node does not exist.
     /// @param[in] node node to be reparented, must not be nullptr.
     /// @param[in] parent new parent of the node, must not be nullptr.
     void reparent_node( Node2D *node, Node2D *parent );

     /// @brief Make node direct child of scene state.
     /// @throws std::runtime_error if state with given @b order does not exist.
     /// @param[in] node node to be reparented, must not be nullptr.
     /// @param[in] order order of the state which will become direct parent of node.
     void reparent_node_to_state( Node2D *node, int order );

     /// @brief Get pointer to node by name, returns nullptr if node with given name does not exist.
     /// @param[in] name name of the node.
     /// @return pointer to node, nullptr if node with given name does not exist.
     //Node2D *get_node( const std::string& name ) const;

     /// @brief Set name for given node, if a node with this name already exists, it will loose name.
     /// @param[in] node pointer to node to be named.
     /// @param[in] name name of the node.
     //void set_node_name( Node2D *node, const std::string& name );

     /// @brief Delete node's name, the node will not be deleted.
     /// @param[in] name name of the node.
     //void delete_node_name( const std::string& name );

private:
     std::map< int, SceneState > states_;    ///< states of this scene with their order.
     SetupFuncPtr setup_func_;               ///< pointer to current scene's setup function.
     LoopFuncPtr loop_func_;                 ///< pointer to current scene's loop function.
};


/// @brief Get current executing scene.
/// @return Current executing scene.
static Scene& get_scene();

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_SCENE_H
