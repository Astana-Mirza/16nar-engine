#include <16nar/game.h>

#include <16nar/tools/flatbuffers_asset_reader.h>
#include <16nar/system/window.h>
#include <16nar/system/iprofile.h>
#include <16nar/system/iscene_reader.h>
#include <16nar/render/irender_api.h>
#include <16nar/logger/logger.h>

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

Game::Game():
     pkg_manager_{ std::make_unique< tools::FlatBuffersAssetReader >() },
     profile_{},
     render_api_{},
     scene_reader_{},
     window_{}
{
     init();
}


Game::~Game()
{
     deinit();
}


void Game::finalize()
{
     pkg_manager_.clear();
     scene_reader_.reset();
     render_api_.reset();
     profile_.reset();
     window_.reset();
}


void Game::set_render_api( std::unique_ptr< IRenderApi >&& render_api )
{
     render_api_ = std::move( render_api );
}


void Game::set_window( std::unique_ptr< Window >&& window )
{
     window_ = std::move( window );
}


void Game::set_scene_reader( std::unique_ptr< ISceneReader >&& scene_reader )
{
     scene_reader_ = std::move( scene_reader );
}


void Game::set_profile( std::unique_ptr< IProfile >&& profile )
{
     profile_ = std::move( profile );
}


Game& Game::instance()
{
     static Game instance_{};
     return instance_;
}


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


IRenderApi& Game::get_render_api()
{
     if (!render_api_)
     {
          throw std::runtime_error{ "game has no render API" };
     }
     return *render_api_;
}


Window& Game::get_window()
{
     if ( !window_ )
     {
          throw std::runtime_error{ "game has no window" };
     }
     return *window_;
}


ISceneReader& Game::get_scene_reader()
{
     if ( !scene_reader_ )
     {
          throw std::runtime_error{ "game has no scene reader" };
     }
     return *scene_reader_;
}


PackageManager& Game::get_pkg_manager()
{
     return pkg_manager_;
}

} // namespace _16nar
