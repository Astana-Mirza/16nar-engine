#include <game.h>

#include <system/file_scene_reader.h>

namespace _16nar
{


Game::Game():
     scene_reader_{ std::make_unique< FileSceneReader >() },
     event_manager_{ std::make_unique< EventManager >() },
     time_per_frame_{ 1.f / 60.f },
     current_task_{ TaskType::Running } {}


Game& Game::get_game()
{
     static Game instance;
     return instance;
}


WorldNode& Game::get_world()
{
     return get_game().world_;
}


void Game::run()
{
     if ( current_task_ == TaskType::Loading )
     {
          world_.load( *scene_reader_ );
     }
     current_task_ = TaskType::Running;
     std::chrono::steady_clock clock;
     world_.setup();
     auto prev_time = clock.now();
     while ( current_task_ != TaskType::Exiting )
     {
          if ( current_task_ == TaskType::Loading )
          {
               world_.load( *scene_reader_ );
               world_.setup();
               current_task_ = TaskType::Running;
               prev_time = clock.now();
          }
          while ( clock.now() - prev_time >= time_per_frame_ )
          {
               read_events();
               world_.loop( time_per_frame_.count() );
               prev_time = clock.now();
          }
          render();
          if ( current_task_ == TaskType::Exiting )
          {
               window_.close();
          }
     }
}


void Game::exit()
{
     current_task_ = TaskType::Exiting;
}


void Game::load_scene( const std::string& name )
{
     scene_reader_ = std::move( std::make_unique< FileSceneReader >() );
     scene_reader_->set_scene( name );
     current_task_ = TaskType::Loading;
}


void Game::set_window( const std::string& title, unsigned width, unsigned height, uint32_t flags, unsigned bits_per_pixel )
{
     window_.create( { width, height, bits_per_pixel }, title, flags );
}


EventManager& Game::get_event_manager()
{
     return *event_manager_;
}


void Game::render()
{
     window_.clear();
     world_.render( window_ );
     window_.display();
}


void Game::read_events()
{
     event_manager_->clear_events();
     Event event;
     while ( window_.pollEvent( event ) )
     {
          if ( event.type != Event::Closed )
          {
               event_manager_->handle_event( event );
          }
          else
          {
               exit();
          }
     }
}


} // namespace _16nar
