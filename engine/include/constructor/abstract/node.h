/// @file
/// File with Node abstract class definition.
#ifndef _16NAR_NODE_H
#define _16NAR_NODE_H

#include <set>
#include <string>

#include <abstract/transformable.h>
#include <abstract/signallable.h>

namespace _16nar
{

/// Abstract base class for all nodes in the engine.
class Node : public Signallable, public Transformable
{
public:
     friend class Game;

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

     std::set< Node * > children_;           ///< set of this node's children.
     std::string *name_ptr_  = nullptr;      ///< pointer to this node's name.
     Node *parent_           = nullptr;      ///< pointer to this node's parent node.
};

} // namespace _16nar

#endif // #ifndef _16NAR_NODE_H
