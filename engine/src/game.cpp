#include <16nar/game.h>

#include <16nar/logger/logger.h>

/*
#include <16nar/system/file_scene_reader.h>
#define USE_SINGLE_THREAD_PROFILE
#ifdef USE_SINGLE_THREAD_PROFILE
#    include <16nar/profiles/single_thread_profile.h>
#endif*/

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace // anonymous
{

void glfw_error_handler( int error_code, const char *description )
{
     throw std::runtime_error{ std::string{ "GLFW error " }
                             + std::to_string( error_code )
                             + ": " + description };
}

} // anonymous namespace


namespace _16nar
{

/*
Game::Game() :
     window_{ std::make_shared< RenderWindow >() },
#ifdef USE_SINGLE_THREAD_PROFILE
     profile_{ std::make_unique< SingleThreadProfile >( world_, window_ ) },
#endif
     scene_reader_{ std::make_unique< FileSceneReader >() },
     event_manager_{ std::make_unique< EventManager >() },
     current_task_{ TaskType::Running }
{
     init();
}


Game& Game::get_game()
{
     static Game instance;
     return instance;
}


WorldNode& Game::get_world()
{
     return get_game().world_;
}

*/
void Game::init()
{
     glfwSetErrorCallback( glfw_error_handler );
     if ( !glfwInit() )
     {
          throw std::runtime_error{ "GLFW error: failed to initialize" };
     }
     LOG_16NAR_INFO( "16nar engine was initialized" );
}


void Game::deinit()
{
     glfwTerminate();
     LOG_16NAR_INFO( "16nar engine was deinitialized" );
}

/*
Game::~Game()
{
     deinit();
}


void Game::run()
{
     if ( current_task_ == TaskType::Loading )
     {
          world_.load( *scene_reader_ );
     }
     current_task_ = TaskType::Running;
     profile_->set_finish( false );
     while ( current_task_ != TaskType::Exiting )
     {
          if ( current_task_ == TaskType::Loading )
          {
               world_.load( *scene_reader_ );
               current_task_ = TaskType::Running;
               profile_->set_finish( false );
          }
          profile_->run();
          if ( current_task_ == TaskType::Exiting )
          {
               window_->close();
               world_.clear();
               profile_.reset();
               scene_reader_.reset();
               event_manager_.reset();
               window_.reset();
          }
     }
}


void Game::exit()
{
     current_task_ = TaskType::Exiting;
     profile_->set_finish( true );
}


void Game::load_scene( const std::string& name )
{
     scene_reader_ = std::move( std::make_unique< FileSceneReader >() );
     scene_reader_->set_scene( name );
     current_task_ = TaskType::Loading;
     profile_->set_finish( true );
}


void Game::read_events()
{
     event_manager_->clear_events();
     Event event;
     while (window_->pollEvent(event))
     {
          if (event.type != Event::Closed)
          {
               event_manager_->handle_event(event);
          }
          else
          {
               exit();
          }
     }
}


void Game::set_window( const std::string& title, unsigned width, unsigned height, uint32_t flags, unsigned bits_per_pixel )
{
     window_->create( { width, height, bits_per_pixel }, title, flags );
}


EventManager& Game::get_event_manager()
{
     return *event_manager_;
}


Texture& Game::get_texture( ResourceID id )
{
     return scene_reader_->get_texture( id );
}


SoundBuffer& Game::get_sound( ResourceID id )
{
     return scene_reader_->get_sound( id );
}


Font& Game::get_font( ResourceID id )
{
     return scene_reader_->get_font( id );
}


Shader& Game::get_shader( ResourceID id )
{
     return scene_reader_->get_shader( id );
}
*/
} // namespace _16nar
