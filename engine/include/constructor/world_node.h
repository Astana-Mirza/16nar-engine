/// @file
/// Header file with WorldNode class definition.
#ifndef _16NAR_WORLD_NODE_H
#define _16NAR_WORLD_NODE_H

#include <constructor/states/scene_state.h>
#include <map>
#include <memory>

namespace _16nar
{

class SceneReader;

/// The world, root of the scene tree. The world is a finite state machine,
/// game states (e.g. main game, pause, mini-game inside main game) can have
/// their own rengering and physics handling.
class ENGINE_API WorldNode
{
public:
     WorldNode()                   = default;
     WorldNode( const WorldNode& ) = delete;
     WorldNode( WorldNode&& )      = delete;

     using SetupFuncPtr = void ( * )( );
     using LoopFuncPtr = void ( * )( float );

     /// Load the scene world with reader.
     /// @param reader object that will load the scene.
     void load( SceneReader& reader );

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

     /// Gets pointer to node by name, returns nullptr if no such name.
     /// @param name name of the node.
     Node* get_node( const std::string& name ) const;

     /// Sets name for given node, previous node with this name will loose it.
     /// @param node pointer to node to be named.
     /// @param name name of the node.
     void set_node_name( Node* node, const std::string& name );

     /// Deletes node's name, the node will not be deleted.
     /// @param name name of the node.
     void delete_node_name( const std::string& name );

private:
     std::unordered_map< std::string, Node * > node_names_;     ///< names of current scene's nodes.
     std::map< int, std::unique_ptr< SceneState > > states_;    ///< states of this scene with their order.
     SetupFuncPtr setup_func_ = nullptr;                        ///< pointer to current scene's setup function.
     LoopFuncPtr loop_func_ = nullptr;                          ///< pointer to current scene's loop function.
};

} // namespace _16nar

#endif // #ifndef _16NAR_WORLD_NODE_H
