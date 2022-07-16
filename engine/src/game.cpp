#include <game.h>

#include <system/dynamic_resource_manager.h>
#include <constructor/sprite_node.h>

#include <iostream>

namespace _16nar
{

Game& Game::get_game()
{
     static Game instance;
     return instance;
}


void Game::run()
{
     Clock clock;
     Time last_update = Time::Zero;
     setup();
     while ( true )
     {
          last_update += clock.restart();
          while ( last_update > time_per_frame_ )
          {
               last_update -= time_per_frame_;
               //read_events();
               loop( time_per_frame_.asSeconds() );
          }
          render();
     }
}


void Game::exit()
{
}


void Game::load_scene( const std::string& name )
{
     scene_name_ = name;
}


Node *Game::get_node( const std::string& name ) const
{
     auto iter = node_names_.find( name );
     if ( iter == node_names_.cend() )
     {
          return nullptr;
     }
     return iter->second;
}


void Game::set_node_name( Node *node, const std::string& name )
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
     node->name_ptr_ = const_cast< std::string * >( &( ret.first->first ) );
}


void Game::delete_node_name( const std::string& name )
{
     auto iter = node_names_.find( name );
     if ( iter != node_names_.end() )
     {
          iter->second->name_ptr_ = nullptr;
          node_names_.erase( iter );
     }
}


void Game::loop( float delta )
{
     if ( loop_func_ )
     {
          loop_func_( delta );
     }
     world_.loop( delta );
}


void Game::setup()
{
     if ( setup_func_ )
     {
          setup_func_();
     }
     world_.setup();
}


void Game::render()
{
     window_.clear();
     world_.render( window_ );
     window_.display();
}


// void Game::read_events()
// {
//      events.clear();
//      Event event;
//      while (window.pollEvent(event))
// 		events.push_back(event);
// }

} // namespace _16nar
