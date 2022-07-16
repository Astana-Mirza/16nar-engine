#include <stdexcept>

#include <constructor/abstract/node.h>
#include <game.h>

namespace _16nar
{

Node::~Node()
{
     if ( name_ptr_ )
     {
          Game::get_game().delete_node_name( *name_ptr_ );
     }
     for ( auto child : children_ )
     {
          delete child;
     }
}


void Node::setup()
{

}


void Node::loop( float delta )
{
     ( void ) delta;
}


void Node::loop_call( bool update, float delta )
{
     loop( delta );
     update = update || transformed_;
     transformed_ = false;
     for ( auto child : get_children() )
     {
          child->loop_call( update, delta );
     }
}


void Node::setup_call()
{
     setup();
     for ( auto child : children_ )
     {
          child->setup_call();
     }
}


void Node::set_parent( Node *parent )
{
     if ( !parent )
     {
          throw std::runtime_error{ "setting null parent" };
     }
     if ( parent_ )
     {
          parent_->remove_child( this );
     }
     parent_ = parent;
     parent_->add_child( this );
}


const Node *Node::get_parent() const
{
     return parent_;
}


const std::set< Node * >& Node::get_children() const
{
     return children_;
}


void Node::add_child( Node *child )
{
     children_.insert( child );
}


void Node::remove_child( Node *child )
{
     children_.erase( child );
}


TransformMatrix Node::get_global_transform_matr() const
{
     TransformMatrix transform = get_transform_matr();
     for ( const Node *n = get_parent(); n != nullptr; n = n->get_parent() )
     {
          transform = n->get_transform_matr() * transform;
     }
     return transform;
}

} // namespace _16nar
