/// @file
/// @brief Header file with WorldNode class definition.
#ifndef _16NAR_WORLD_NODE_H
#define _16NAR_WORLD_NODE_H

#include <16nar/constructor/states/scene_state.h>
#include <map>
#include <memory>

namespace _16nar
{

class SceneReader;

/// @brief The world, root of the scene tree.
/// @detail The world is a finite state machine, game states (e.g. main game, pause, hp bar)
/// can have their own rengering and physics handling.
class ENGINE_API WorldNode
{
public:
     WorldNode()                   = default;
     WorldNode( const WorldNode& ) = delete;
     WorldNode( WorldNode&& )      = delete;

     using SetupFuncPtr = void ( * )( );
     using LoopFuncPtr = void ( * )( float );

     /// @brief Delete all states and node names.
     void clear();

     /// @brief Load the scene world with reader.
     /// @param[in] reader object that will load the scene.
     void load( SceneReader& reader );

     /// @brief Setup function called once.
     void setup();

     /// @brief Loop function called in game loop.
     /// @param[in] delta time since previous loop call, in seconds.
     void loop( float delta );

     /// @brief Start rendering of all states on the target.
     /// @param[in] target target on which everething will be drawn.
     void start_render( RenderTarget& target );

     /// @brief Finish rendering of all states on the target.
     void finish_render();

     /// @brief Register new simple scene state.
     /// @param[in] order order value which defines order of state updating.
     /// @param[in] state pointer to scene state.
     void register_state( int order, std::unique_ptr< SceneState >&& state );

     /// @brief Gets the scene state with given order, throws exception if no such state.
     /// @param[in] order order of the state.
     SceneState& get_state( int order ) const;

     /// @brief Gets pointer to node by name, returns nullptr if no such name.
     /// @param[in] name name of the node.
     Node* get_node( const std::string& name ) const;

     /// @brief Sets name for given node, previous node with this name will loose it.
     /// @param[in] node pointer to node to be named.
     /// @param[in] name name of the node.
     void set_node_name( Node* node, const std::string& name );

     /// @brief Deletes node's name, the node will not be deleted.
     /// @param name[in] name of the node.
     void delete_node_name( const std::string& name );

private:
     std::unordered_map< std::string, Node * > node_names_;     ///< names of current scene's nodes.
     std::map< int, std::unique_ptr< SceneState > > states_;    ///< states of this scene with their order.
     SetupFuncPtr setup_func_ = nullptr;                        ///< pointer to current scene's setup function.
     LoopFuncPtr loop_func_ = nullptr;                          ///< pointer to current scene's loop function.
};

} // namespace _16nar

#endif // #ifndef _16NAR_WORLD_NODE_H
