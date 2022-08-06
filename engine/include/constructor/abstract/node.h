/// @file
/// File with Node abstract class definition.
#ifndef _16NAR_NODE_H
#define _16NAR_NODE_H

#include <set>
#include <string>
#include <unordered_map>
#include <memory>

#include <constructor/animations/animation.h>
#include <abstract/transformable.h>
#include <abstract/signallable.h>

namespace _16nar
{

class Animation;

/// Abstract base class for all nodes in the engine.
class ENGINE_API Node : public Signallable, public Transformable
{
public:
     using AnimationMap = std::unordered_map< std::string, std::unique_ptr< Animation > >;
     friend class WorldNode;

     Node()              = default;
     Node( const Node& ) = delete;
     Node( Node&& )      = delete;

     /// Virtual destructor, which deletes node's name and children.
     virtual ~Node();

     /// Sets parent of this node.
     /// @param pointer to new parent node.
     void set_parent( Node *parent );

     /// Gets parent of this node.
     const Node *get_parent() const;

     /// Gets set of all first-order children (no grandchildren) of this node.
     const std::set< Node * >& get_children() const;

     /// Gets global transform matrix of this node.
     TransformMatrix get_global_transform_matr() const;

     /// Function that launches setup recursively.
     void setup_call();

     /// Function that launches update loop recursively.
     /// @param upd value, telling if this branch of scene tree has to be updated.
     /// @param delta time since last update, in seconds.
     virtual void loop_call( bool upd, float delta );

     /// Adds an animation to this node.
     /// @param name name of the animation.
     /// @param anim pointer to the animation.
     void add_animation( const std::string& name, std::unique_ptr< Animation >&& anim );

     /// Deletes animation with given name.
     /// @param name name of the animation.
     void delete_animation( const std::string& name );

     /// Gets animation with given name, nullptr if no animation with given name.
     /// @param name name of the animation.
     Animation *get_animation( const std::string& name ) const;

protected:
     /// Function to be executed at scene startup.
     virtual void setup();

     /// Function to be executed at scene update.
     /// @param delta time since last update, in seconds.
     virtual void loop( float delta );

private:
     /// Adds child to set of children.
     /// @param child pointer to new child node.
     void add_child( Node *child );

     /// Removes child from set of children, memory will not be freed.
     /// @param child pointer to the child to be removed.
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
