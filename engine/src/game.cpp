#include <16nar/game.h>

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
     pkg_manager_{},
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
