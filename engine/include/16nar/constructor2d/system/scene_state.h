/// @file
/// @brief Header file with SceneState class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_SCENE_STATE_H
#define _16NAR_CONSTRUCTOR_2D_SCENE_STATE_H

#include <16nar/constructor2d/render/irender_system_2d.h>
#include <16nar/constructor2d/node_2d.h>

namespace _16nar::constructor2d
{

/// @brief Simple state of scene which takes update and render flow.
class ENGINE_API SceneState
{
public:
     friend class Scene;

     /// @brief Constructor, sets updating and rendering of the state.
     /// @param[in] order order of state in scene workflow.
     /// @param[in] render_system render system for the state.
     /// @param[in] updating set if this state will be updated in game loop.
     /// @param[in] rendering set if this state will be rendered in game loop.
     explicit SceneState( int order, std::unique_ptr< IRenderSystem2D >&& render_system,
                 bool updating = true, bool rendering = true );

     /// @brief Get order of this state.
     /// @return order of this state.
     int get_order() const noexcept;

     /// @brief Set the updating flag.
     /// @param[in] rendering set if this state will be rendered in game loop.
     void set_rendering( bool rendering ) noexcept;

     /// @brief Set the updating flag.
     /// @param[in] updating set if this state will be updated in game loop.
     void set_updating( bool updating ) noexcept;

     /// @brief Get the updating flag.
     /// @return updating flag.
     bool get_updating() const noexcept;

     /// @brief Get the rendering flag.
     /// @return rendering flag.
     bool get_rendering() const noexcept;

     /// @brief Get reference to the render system.
     IRenderSystem2D& get_render_system();

     /// @brief Create node and make it direct child of this state.
     /// @details Node will be constructed using "()" braces (not "{}"), as with std::make_unique.
     /// @tparam NodeType type of child node.
     /// @tparam Params types of parameters for node construction.
     /// @param[in] params parameters for node construction.
     /// @return pointer to created node.
     template < typename NodeType, typename... Params >
     NodeType *create_child( Params&&... params )
     {
          static_assert( std::is_constructible_v< std::decay_t< NodeType >, Params... >,
               "NodeType must be constructible with Params" );
          auto ptr = std::make_unique< NodeType >( std::forward< Params >( params )... );
          NodeType *raw_ptr = ptr.get();
          add_child( std::move( ptr ) );
          return raw_ptr;
     }

private:
     /// @brief Execute all objects' setup functions.
     /// @details Should not be called by user, unless you are sure about that.
     void setup();

     /// @brief Execute all objects' loop functions.
     /// @details Should not be called by user, unless you are sure about that.
     /// @param[in] delta time since last update, in seconds.
     void loop( float delta );

     /// @brief Add the child node, it will have no parent.
     /// @details After this function is called, new state order must be saved in @b node.
     /// Should not be called by user, unless you are sure about that.
     /// So, @ref Node2D::set_state_order(int) noexcept must be called with current state order.
     /// However, for adding existing nodes @ref Scene::reparent_node_to_state(Node2D*, int)
     /// should be used (it does equivalent tasks).
     /// @throws std::runtime_error if insertion fails (e.g. @b node pointer is already stored).
     /// @param[in] node pointer to node to be added.
     void add_child( std::unique_ptr< Node2D >&& node );

     /// @brief Remove the node from ownership and return it, node will not be destructed and deleted.
     /// @details Should not be called by user, unless you are sure about that.
     /// @param[in] node pointer to node to be removed.
     /// @return unique pointer to removed node, nullptr if @b node is not direct child of this state.
     std::unique_ptr< Node2D > remove_child( const Node2D *node );

private:
     std::unique_ptr< IRenderSystem2D > render_system_;     ///< render system of this state.
     NodesList nodes_;              ///< set of this state's direct children nodes.
     int order_;                    ///< order of state in scene workflow.
     bool updating_;                ///< set if this state will be rendered in game loop.
     bool rendering_;               ///< set if this state will be rendered in game loop.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_SCENE_STATE_H
