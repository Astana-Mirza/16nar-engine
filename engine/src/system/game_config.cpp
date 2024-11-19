#include <16nar/system/game_config.h>

#include <stdexcept>

#ifdef __linux__
#    include <cstdlib>
#    include <unistd.h>
#    include <linux/limits.h>
#elif _WIN32
#    include <windows.h>
#    include <libloaderapi.h>
#    include <shlobj.h>
#endif

#include <nlohmann/json.hpp>

namespace _16nar
{

namespace tools
{

NLOHMANN_JSON_SERIALIZE_ENUM( PackageFormat, {
     { PackageFormat::Json,             "json" },
     { PackageFormat::FlatBuffers,      "flatbuffers" },
} )

} // namespace tools

NLOHMANN_JSON_SERIALIZE_ENUM( ProfileType, {
     { ProfileType::Unknown,            nullptr },
     { ProfileType::SingleThreaded,     "single_threaded" },
     { ProfileType::MultiThreaded,      "multi_threaded" },
} )


GameConfig::GameConfig() noexcept:
     app_data_dir{ "." }, app_dir{ "." }, app_name{},
     render_api{ RenderApiType::Unknown },
     profile{ ProfileType::Unknown },
     log_level{ ILogWriter::LogLevel::Info },
     resources_format{ tools::PackageFormat::FlatBuffers },
     resources_unpacked{ false }
{}


GameConfig::GameConfig( const std::string& name ):
     app_data_dir{}, app_dir{}, app_name{ name },
     render_api{ RenderApiType::Unknown },
     profile{ ProfileType::Unknown },
     log_level{ ILogWriter::LogLevel::Info },
     resources_format{ tools::PackageFormat::FlatBuffers },
     resources_unpacked{ false }
{
     app_data_dir = ( get_app_data_dir() / app_name ).string();
     app_dir = get_app_dir().string();
}


GameConfig GameConfig::load_json( std::istream& input )
{
     GameConfig config{};
     auto json = nlohmann::json::parse( input );
     config.app_data_dir = json.at( "app_data_dir" );
     config.app_dir = json.at( "app_dir" );
     config.app_name = json.at( "app_name" );

     std::string render_api = json.at( "render_api" );
#if defined( NARENGINE_RENDER_OPENGL )
     if ( render_api == "open_gl" )
     {
          config.render_api = RenderApiType::OpenGl;
     }
     else
#endif // NARENGINE_RENDER_OPENGL
     {
          config.render_api = RenderApiType::Unknown;
     }
     config.profile = json.at( "profile" ). template get< ::_16nar::ProfileType >();
     config.log_level = static_cast< ILogWriter::LogLevel >( json.at( "log_level" ).get< std::size_t >() );
     config.resources_format = json.at( "resources_format" ). template get< ::_16nar::tools::PackageFormat >();
     config.scene_format = json.at( "scene_format" ). template get< ::_16nar::tools::PackageFormat >();
     config.resources_unpacked = json.at( "resources_unpacked" );
     return config;
}


std::filesystem::path GameConfig::get_app_dir()
{
#ifdef __linux__
     std::string link_path = "/proc/" + std::to_string( ::getpid() ) + "/exe";
     // useless to check real link size with lstat() because using /proc always return st_size == 0
     char path[ PATH_MAX ] = {};
     auto nbytes = ::readlink( link_path.c_str(), path, PATH_MAX );
     if ( nbytes <= 0 )
     {
          throw std::runtime_error{ "cannot get application directory" };
     }
     path[ nbytes >= PATH_MAX ? PATH_MAX - 1 : nbytes ] = '\0';
     return std::filesystem::path{ path }.parent_path();
#elif _WIN32
     WCHAR path[ MAX_PATH ];
     if ( 0 == GetModuleFileNameW( NULL, path, MAX_PATH ) )
     {
          throw std::runtime_error{ "cannot get application directory" };
     }
     return std::filesystem::path{ path }.parent_path();
#else
     return std::filesystem::path{};
#endif
}


std::filesystem::path GameConfig::get_app_data_dir()
{
#ifdef __linux__
     std::filesystem::path path;
     const char *base_dir = std::getenv( "XDG_DATA_HOME" );
     if ( base_dir == nullptr )
     {
          base_dir = std::getenv( "HOME" );
          if ( base_dir == nullptr )
          {
               throw std::runtime_error{ "cannot get application data directory" };
          }
          path = std::filesystem::path{ base_dir } / ".local" / "share";
     }
     else
     {
          path = std::filesystem::path{ base_dir };
     }
     if ( !std::filesystem::exists( path ) || !std::filesystem::is_directory( path ) )
     {
          throw std::runtime_error{ "cannot get application data directory " + path.string() };
     }
     return path;
#elif _WIN32
     PWSTR path;
     auto ret = ::SHGetKnownFolderPath( FOLDERID_RoamingAppData, 0, nullptr, &path );
     if ( ret != S_OK )
     {
          ::CoTaskMemFree( path );
          throw std::runtime_error{ "cannot get application data directory" };
     }
     std::filesystem::path result{ path };
     ::CoTaskMemFree( path );
     return result;
#else
     return std::filesystem::path{};
#endif
}

} // namespace _16nar
