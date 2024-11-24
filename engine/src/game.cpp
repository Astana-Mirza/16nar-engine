#include <16nar/game.h>

#include <16nar/system/window.h>
#include <16nar/system/iprofile.h>
#include <16nar/system/iscene_reader.h>
#include <16nar/logger/logger.h>

#include <16nar/tools/json_asset_reader.h>
#include <16nar/tools/flatbuffers_asset_reader.h>

#if defined( NARENGINE_RENDER_OPENGL )
#    include <16nar/render/opengl/render_api.h>
#endif // NARENGINE_RENDER_OPENGL

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


std::unique_ptr< _16nar::tools::IAssetReader > create_asset_reader( const std::string& in_dir,
     _16nar::tools::PackageFormat format )
{
     switch ( format )
     {
          case _16nar::tools::PackageFormat::FlatBuffers:
               return std::make_unique< _16nar::tools::FlatBuffersAssetReader >();
          case _16nar::tools::PackageFormat::Json:
               return std::make_unique< _16nar::tools::JsonAssetReader >( in_dir );
          default:
               throw std::runtime_error{ "wrong asset reader format: "
                    + std::to_string( static_cast< std::size_t >( format ) ) };
     }
     return {};
}

} // anonymous namespace


namespace _16nar
{

bool Game::initialized_ = false;
GameConfig Game::config_{};


Game::Game():
     pkg_manager_{},
     profile_{},
     render_api_{},
     scene_reader_{},
     window_{}
{
     init();   // initialize with default config, if not initialized earlier.

     // call for correct initialization order
     Logger::instance().log( ILogWriter::LogLevel::Info, "creating game object" );

     pkg_manager_ = std::make_unique< PackageManager >( create_asset_reader(
          config_.app_dir, config_.resources_format ) );
     pkg_manager_->set_package_dir( config_.app_dir );
     pkg_manager_->set_unpacked_mode( config_.resources_unpacked );
}


Game::~Game()
{
     finalize();
     deinit();
}


void Game::finalize()
{
     pkg_manager_.reset();
     scene_reader_.reset();
     render_api_.reset();
     profile_.reset();
     window_.reset();
}


void Game::set_render_api( std::unique_ptr< IRenderApi >&& render_api ) noexcept
{
     render_api_ = std::move( render_api );
}


void Game::set_window( std::unique_ptr< Window >&& window ) noexcept
{
     window_ = std::move( window );
}


void Game::set_scene_reader( std::unique_ptr< ISceneReader >&& scene_reader ) noexcept
{
     scene_reader_ = std::move( scene_reader );
}


void Game::set_profile( std::unique_ptr< IProfile >&& profile ) noexcept
{
     profile_ = std::move( profile );
}


Game& Game::instance()
{
     static Game instance_{};
     return instance_;
}


void Game::init( const GameConfig& config )
{
     if ( initialized_ )
     {
          return;
     }
     config_ = config;
     Logger::instance().set_log_level( config_.log_level );
     if ( !std::filesystem::exists( config_.app_data_dir )
          && !std::filesystem::create_directories( config_.app_data_dir ) )
     {
          throw std::runtime_error{ "cannot create directory " + config_.app_data_dir };
     }
     else if ( !std::filesystem::is_directory( config_.app_data_dir ) )
     {
          throw std::runtime_error{ config_.app_data_dir + " already exists and is not a directory" };
     }
     glfwSetErrorCallback( glfw_error_handler );
     if ( !glfwInit() )
     {
          throw std::runtime_error{ "GLFW error: failed to initialize" };
     }
     initialized_ = true;
     LOG_16NAR_INFO( "16nar engine was initialized" );
}


void Game::deinit()
{
     if ( !initialized_ )
     {
          return;
     }
     glfwTerminate();
     initialized_ = false;
     LOG_16NAR_INFO( "16nar engine was deinitialized" );
}


const GameConfig& Game::get_config() const noexcept
{
     return config_;
}


IRenderApi& Game::get_render_api() noexcept
{
     return *render_api_;
}


Window& Game::get_window() noexcept
{
     return *window_;
}


ISceneReader& Game::get_scene_reader() noexcept
{
     return *scene_reader_;
}


PackageManager& Game::get_pkg_manager() noexcept
{
     return *pkg_manager_;
}

} // namespace _16nar
