/// @file
/// @brief File with Node abstract class definition.
#ifndef _16NAR_NODE_H
#define _16NAR_NODE_H

#include <set>
#include <string>
#include <unordered_map>
#include <memory>

#include <16nar/constructor/animations/animation.h>
#include <16nar/abstract/transformable_2d.h>
#include <16nar/abstract/signallable.h>

namespace _16nar
{

class Animation;

/// @brief Abstract base class for all nodes in the engine.
class ENGINE_API Node : public Signallable, public Transformable2D
{
public:
     using AnimationMap = std::unordered_map< std::string, std::unique_ptr< Animation > >;
     friend class WorldNode;

     Node()              = default;
     Node( const Node& ) = delete;
     Node( Node&& )      = delete;

     /// @brief Virtual destructor, which deletes node's name and children.
     virtual ~Node();

     /// @brief Sets parent of this node.
     /// @param[in] pointer to new parent node.
     void set_parent( Node *parent );

     /// @brief Gets parent of this node.
     const Node *get_parent() const;

     /// @brief Gets set of all first-order children (no grandchildren) of this node.
     const std::set< Node * >& get_children() const;

     /// @brief Gets global transform matrix of this node.
     /// @param[in] include_local should the local transform be included in calculation
     TransformMatrix get_global_transform_matr( bool include_local = true ) const;

     /// @brief Function that launches setup recursively.
     void setup_call();

     /// @brief Function that launches update loop recursively.
     /// @param[in] upd value, telling if this branch of scene tree has to be updated.
     /// @param[in] delta time since last update, in seconds.
     virtual void loop_call( bool upd, float delta );

     /// @brief Adds an animation to this node.
     /// @param[in] name name of the animation.
     /// @param[in] anim pointer to the animation.
     void add_animation( const std::string& name, std::unique_ptr< Animation >&& anim );

     /// @brief Deletes animation with given name.
     /// @param[in] name name of the animation.
     void delete_animation( const std::string& name );

     /// @brief Gets animation with given name, nullptr if no animation with given name.
     /// @param[in] name name of the animation.
     Animation *get_animation( const std::string& name ) const;

protected:
     /// @brief Function to be executed at scene startup.
     virtual void setup();

     /// @brief Function to be executed at scene update.
     /// @param[in] delta time since last update, in seconds.
     virtual void loop( float delta );

private:
     /// @brief Adds child to set of children.
     /// @param[in] child pointer to new child node.
     void add_child( Node *child );

     /// @brief Removes child from set of children, memory will not be freed.
     /// @param[in] child pointer to the child to be removed.
     void remove_child( Node *child );

protected:
     AnimationMap animations_;               ///< all animations of this node.

private:
     std::set< Node * > children_;           ///< set of this node's children.
     std::string *name_ptr_  = nullptr;      ///< pointer to this node's name.
     Node *parent_           = nullptr;      ///< pointer to this node's parent node.
};

} // namespace _16nar

#endif // #ifndef _16NAR_NODE_H
