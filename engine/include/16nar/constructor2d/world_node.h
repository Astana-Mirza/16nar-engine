/// @file
/// @brief Header file with WorldNode class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_WORLD_NODE_H
#define _16NAR_CONSTRUCTOR_2D_WORLD_NODE_H

#include <map>
#include <memory>


namespace _16nar::constructor2d
{

class SceneState;


/// @brief The world, root of the scene tree.
/// @detail The world is a finite state machine, game states (e.g. main game, pause, hp bar)
/// can have their own rengering and physics handling.
class ENGINE_API WorldNode
{
public:
     /// @brief Get current executing world.
     /// @return Current executing world.
     static WorldNode& get_world();

     WorldNode()                   = default;
     WorldNode( const WorldNode& ) = delete;

     using SetupFuncPtr = void ( * )( );
     using LoopFuncPtr = void ( * )( float );

     /// @brief Swap world content with another world.
     /// @param[in] other world to exchange content.
     void swap( WorldNode& other );

     /// @brief Setup function called once.
     void setup();

     /// @brief Loop function called in game loop.
     /// @param[in] delta time since previous loop call, in seconds.
     void loop( float delta );

     /// @brief Get render system for drawing the world.
     /// @return render system for drawing the world.
     IRenderSystem *get_render_system() const;

     /// @brief Start rendering of all states on the target.
     /// @param[in] device device with which everething will be rendered.
     void start_render( RenderDevice& device );

     /// @brief Finish rendering of all states on the target.
     /// @param[in] device device with which everething will be rendered.
     void finish_render( RenderDevice& device );

     /// @brief Register new simple scene state.
     /// @param[in] order order value which defines order of state updating.
     /// @param[in] state pointer to scene state.
     void register_state( int order, std::unique_ptr< SceneState >&& state );

     /// @brief Get the scene state with given order, throws exception if no such state.
     /// @param[in] order order of the state.
     SceneState& get_state( int order ) const;

     /// @brief Get pointer to node by name, returns nullptr if no such name.
     /// @param[in] state_num index of state in which the node is located.
     /// @param[in] name name of the node.
     Node *get_node( int state_num, std::string_view name ) const;

     /// @brief Set name for given node, previous node with this name will loose it.
     /// @param[in] node pointer to node to be named.
     /// @param[in] name name of the node.
     void set_node_name( Node *node, const std::string& name );

     /// @brief Delete node's name, the node will not be deleted.
     /// @param name[in] name of the node.
     void delete_node_name( const std::string& name );

private:
     std::map< int, std::unique_ptr< SceneState > > states_;     ///< states of this scene with their order.
     SetupFuncPtr setup_func_ = nullptr;                         ///< pointer to current scene's setup function.
     LoopFuncPtr loop_func_ = nullptr;                           ///< pointer to current scene's loop function.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_WORLD_NODE_H
