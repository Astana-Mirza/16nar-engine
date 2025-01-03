#include <16nar/constructor2d/system/scene.h>

#include <16nar/constructor2d/node_2d.h>
#include <16nar/constructor2d/system/scene_state.h>

#include <stdexcept>
#include <cassert>

namespace _16nar::constructor2d
{
namespace
{

std::unique_ptr< Node2D > extract_node( std::map< int, SceneState >& states, Node2D *node )
{
     Node2D *prev_parent = node->get_parent();
     int prev_order = node->get_state_order();
     std::unique_ptr< Node2D > ptr{};

     if ( !prev_parent )
     {
          auto state_iter = states.find( prev_order );
          if ( state_iter == states.end() )
          {
               throw std::runtime_error{ "node has no parent and has unknown state order "
                    + std::to_string( prev_order ) };
          }
          ptr = state_iter->second.remove_node( node );
          if ( !ptr )
          {
               throw std::runtime_error{ "node's saved state does not have this node as child" };
          }
     }
     else
     {
          ptr = prev_parent->remove_child( node );
          if ( !ptr )
          {
               throw std::runtime_error{ "node's parent does not have this node as child" };
          }
     }
     return ptr;
}

} // anonymous namespace

Scene::Scene():
     states_{}, setup_func_{ nullptr }, loop_func_{ nullptr }
{}


void Scene::swap( Scene& other )
{
     //other.node_names_.swap( node_names_ );
     other.states_.swap( states_ );
     std::swap( other.setup_func_, setup_func_ );
     std::swap( other.loop_func_, loop_func_ );
}


void Scene::setup()
{
     if ( setup_func_ )
     {
          setup_func_();
     }
     for ( auto& [ order, state ] : states_ )
     {
          state.setup();
     }
}


void Scene::loop( float delta )
{
     if ( loop_func_ )
     {
          loop_func_( delta );
     }
     for ( auto& [ order, state ] : states_ )
     {
          if ( state.get_updating() )
          {
               state.loop( delta );
          }
     }
}


void Scene::register_state( int order, SceneState&& state )
{
     states_.emplace( order, std::move( state ) );
}


const SceneState& Scene::get_state( int order ) const
{
     return states_.at( order );
}


SceneState& Scene::get_state( int order )
{
     return states_.at( order );
}


void Scene::reparent_node( Node2D *node, Node2D *parent )
{
     assert( node );
     assert( parent );

     if ( node->get_parent() == parent )
     {
          // already reparented
          return;
     }

     auto ptr = extract_node( states_, node );
     parent->add_child( std::move( ptr ) );
}


void Scene::reparent_node_to_state( Node2D *node, int order )
{
     assert( node );

     if ( node->get_state_order() == order && !node->get_parent() )
     {
          // already reparented
          return;
     }

     auto new_state_iter = states_.find( order );
     if ( new_state_iter == states_.end() )
     {
          throw std::runtime_error{ "state with required order " + std::to_string( order ) 
               + "does not exist" };
     }

     auto ptr = extract_node( states_, node );
     ptr->set_state_order( order );
     new_state_iter->second.add_node( std::move( ptr ) );
}


/*Node2D *Scene::get_node( const std::string& name ) const
{
     auto iter = node_names_.find( name );
     if ( iter == node_names_.cend() )
     {
          return nullptr;
     }
     return iter->second;
}


void Scene::set_node_name( Node2D *node, const std::string& name )
{
     auto ret = node_names_.insert( { name, node } );
     if ( !ret.second )
     {
          throw std::runtime_error{ "name \"" + name + "\" already exists" };
     }
     if ( node->name_ptr_ )
     {
          node_names_.erase( *( node->name_ptr_ ) );          // erase old name
     }
     node->name_ptr_ = const_cast< std::string* >( &( ret.first->first ) );
}


void Scene::delete_node_name( const std::string& name )
{
     auto iter = node_names_.find( name );
     if ( iter != node_names_.end() )
     {
          //iter->second->name_ptr_ = nullptr;
          node_names_.erase( iter );
     }
}
*/

namespace
{

Scene current_scene;

} // anonymous namespace


Scene& get_scene()
{
     return current_scene;
}

} // namespace _16nar::constructor2d
