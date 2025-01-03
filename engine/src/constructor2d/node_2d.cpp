#include <16nar/constructor2d/node_2d.h>

#include <16nar/constructor2d/system/scene_state.h>

namespace _16nar::constructor2d
{

Node2D::Node2D():
     children_{}, name_{}, parent_{ nullptr }, setup_func_{ nullptr },
     loop_func_{ nullptr }, state_order_{ -1 }
{}


Node2D *Node2D::get_parent() noexcept
{
     return parent_;
}


const Node2D *Node2D::get_parent() const noexcept
{
     return parent_;
}


const std::string& Node2D::get_name() const noexcept
{
     return name_;
}


TransformMatrix Node2D::get_global_transform_matr( bool include_self ) const noexcept
{
     TransformMatrix transform = include_self ? get_transform_matr() : TransformMatrix{};
     for ( const Node2D *node = get_parent(); node != nullptr; node = node->get_parent() )
     {
          transform = node->get_transform_matr() * transform;
     }
     return transform;
}


const NodesSet& Node2D::get_children() const noexcept
{
     return children_;
}


// Member functions not for user to call


void Node2D::set_state_order( int state_order ) noexcept
{
     state_order_ = state_order;
}


int Node2D::get_state_order() const noexcept
{
     return state_order_;
}


void Node2D::setup_call( SceneState& state )
{
     if ( setup_func_ )
     {
          setup_func_( this, state );
     }
     for ( auto& child : children_ )
     {
          child->setup_call( state );
     }
}


void Node2D::loop_call( SceneState& state, float delta, bool updated )
{
     if ( loop_func_ )
     {
          loop_func_( this, state, delta );
     }
     bool transformed = calculate_matr();
     updated = updated || transformed || updated_;
     updated_ = false;
     for ( auto& child : children_ )
     {
          child->loop_call( state, delta, updated );
     }
}


void Node2D::add_child( std::unique_ptr< Node2D >&& node )
{
     auto pair = children_.insert( std::move( node ) );
     ( *pair.first )->parent_ = this;
     ( *pair.first )->state_order_ = state_order_;
     ( *pair.first )->updated_ = true;
}


std::unique_ptr< Node2D > Node2D::remove_child( Node2D *node )
{
     auto iter = children_.find( node );
     if ( iter != children_.end() )
     {
          auto handle = children_.extract( iter );
          auto ptr{ std::move( handle.value() ) };
          ptr->parent_ = nullptr;
          return ptr;
     }
     return std::unique_ptr< Node2D >{};
}

} // namespace _16nar::constructor2d
