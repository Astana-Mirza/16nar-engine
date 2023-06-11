#include <16nar/constructor/world_node.h>
#include <16nar/abstract/scene_reader.h>

namespace _16nar
{

void WorldNode::clear()
{
     states_.clear();
     setup_func_ = nullptr;
     loop_func_ = nullptr;
}


void WorldNode::load( SceneReader& reader )
{
     reader.load_scene( *this, setup_func_, loop_func_ );
}


void WorldNode::setup()
{
     if ( setup_func_ )
     {
          setup_func_();
     }
     for ( auto& [ order, state ] : states_ )
     {
          state->setup();
     }
}


void WorldNode::loop( float delta )
{
     if ( loop_func_ )
     {
          loop_func_( delta );
     }
     for ( auto& [ order, state ] : states_ )
     {
          if ( state->get_updating() )
          {
               state->loop( delta );
          }
     }
}


void WorldNode::start_render( RenderTarget& target )
{
     for ( auto& [ order, state ] : states_ )
     {
          if ( state->get_rendering() )
          {
               state->start_render( target );
          }
     }
}


void WorldNode::finish_render()
{
     for ( auto& [ order, state ] : states_ )
     {
          if ( state->get_rendering() )
          {
               state->finish_render();
          }
     }
}


void WorldNode::register_state( int order, std::unique_ptr< SceneState >&& state )
{
     states_[ order ] = std::move( state );
}


SceneState& WorldNode::get_state( int order ) const
{
     return *states_.at( order );
}


Node* WorldNode::get_node( const std::string& name ) const
{
     auto iter = node_names_.find( name );
     if ( iter == node_names_.cend() )
     {
          return nullptr;
     }
     return iter->second;
}


void WorldNode::set_node_name( Node* node, const std::string& name )
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


void WorldNode::delete_node_name( const std::string& name )
{
     auto iter = node_names_.find( name );
     if ( iter != node_names_.end() )
     {
          iter->second->name_ptr_ = nullptr;
          node_names_.erase( iter );
     }
}


} // namespace _16nar
