/// @file
/// @brief File with Node2D class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_NODE_2D_H
#define _16NAR_CONSTRUCTOR_2D_NODE_2D_H

#include <16nar/16nardefs.h>
#include <16nar/constructor2d/transformable_2d.h>
#include <16nar/signals/signallable.h>

#include <set>
#include <string>
#include <memory>
#include <type_traits>

namespace _16nar::constructor2d
{

class Node2D;
class SceneState;

/// @brief List of nodes.
using NodesList = std::vector< std::unique_ptr< Node2D > >;

/// @brief A simple node that keep its transformation.
class ENGINE_API Node2D : public Transformable2D, public Signallable
{
public:
     friend class Scene;
     friend class SceneState;

     using SetupFuncPtr = void ( * )( Node2D *, SceneState& );
     using LoopFuncPtr = void ( * )( Node2D *, SceneState&, float );

     Node2D( const Node2D& ) = delete;
     Node2D& operator=( const Node2D& ) = delete;

     /// @brief Default constructor.
     Node2D();

     /// @brief Get current node's parent node.
     /// @return pointer to current node's parent node, nullptr of there is no parent.
     Node2D *get_parent() noexcept;

     /// @copydoc Node2D::get_parent()
     const Node2D *get_parent() const noexcept;

     /// @brief Get name of the node.
     /// @return name of the node.
     const std::string& get_name() const noexcept;

     /// @brief Get global transformation matrix.
     /// @param[in] include_self true if should include local matrix in result, false otherwise.
     /// @return global transformation matrix.
     TransformMatrix get_global_transform_matr( bool include_self = true ) const noexcept;

     /// @brief Get set of direct children of this node.
     /// @return set of direct children of this node.
     const NodesList& get_children() const noexcept;

     /// @brief Create node and make it child of this node.
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

public:
     /// @brief Execute loop function of current node and all children recursively.
     /// @details Should not be called by user, unless you are sure about that.
     /// @param[in] state current node's scene state.
     /// @param[in] delta time passed since previous loop call, in seconds.
     /// @param[in] updated true if node needs to be rerendered after update, false otherwise.
     virtual void loop_call( SceneState& state, float delta, bool updated );

     /// @brief Execute setup function of current node and all children recursively.
     /// @details Should not be called by user, unless you are sure about that.
     /// @param[in] state current node's scene state.
     void setup_call( SceneState& state );

private:
     /// @brief Set name of current node.
     /// @details Must not be called by user, because node's name must also
     /// be registered in scene. So, @ref Scene::set_node_name(Node2D *, const std::string&)
     /// must be called instead.
     /// @param[in] name name to be set.
     void set_name( const std::string& name );

     /// @brief Set order of scene state to which current node belongs.
     /// @param[in] state_order order of scene state.
     void set_state_order( int state_order ) noexcept;

     /// @brief Get order of scene state to which current node belongs.
     /// @details Returned value is valid if and only if node's parent is nullptr,
     /// i.e. when node is direct child of scene state.
     /// @return order of scene state to which current node belongs.
     int get_state_order() const noexcept;

     /// @brief Add node to set of children.
     /// @throws std::runtime_error if insertion fails (e.g. @b node pointer is already stored).
     /// @param[in] node pointer to node to be added.
     void add_child( std::unique_ptr< Node2D >&& node );

     /// @brief Remove the node from ownership and return it, node will not be destructed and deleted.
     /// @param[in] node pointer to node to be removed.
     /// @return unique pointer to removed child node, nullptr if @b node is not direct child of this state.
     std::unique_ptr< Node2D > remove_child( const Node2D *node );

private:
     NodesList children_;          ///< children of current node.
     std::string name_;            ///< name of the node.
     Node2D *parent_;              ///< parent of current node.
     int state_order_;             ///< order of scene state to which current node belongs,
                                   ///< makes sesnse only if node is direct child of state.
protected:
     SetupFuncPtr setup_func_;     ///< pointer to current scene's setup function.
     LoopFuncPtr loop_func_;       ///< pointer to current scene's loop function.
     bool updated_;                ///< flag of node update, sets to true if node was changed in scene tree logic.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_NODE_2D_H
