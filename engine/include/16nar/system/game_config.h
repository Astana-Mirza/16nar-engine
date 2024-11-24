/// @file
/// @brief File with GameConfig class definition.
#ifndef _16NAR_GAME_CONFIG_H
#define _16NAR_GAME_CONFIG_H

#include <16nar/16nardefs.h>
#include <16nar/logger/ilog_writer.h>
#include <16nar/tools/resource_package.h>

#include <string>
#include <iosfwd>
#include <filesystem>

namespace _16nar
{

/// @brief Basic configuration of application.
struct ENGINE_API GameConfig
{
     /// @brief Default constructor.
     GameConfig();

     /// @brief Constructor.
     /// @details Ensures that all directories neccessary for application are created.
     /// @param[in] name name of the application.
     /// @throws std::runtime_error.
     GameConfig( const std::string& name );

     /// @brief Load configuration from JSON format.
     /// @param input input data stream.
     /// @return loaded configuration.
     /// @throws std::runtime_error.
     static GameConfig load_json( std::istream& input );

     /// @brief Get current application directory.
     /// @return current application directory (directory of current binary file).
     /// @throws std::runtime_error.
     static std::filesystem::path get_app_dir();

     /// @brief Get common applications data directory.
     /// @details On Windows, returns %appdata%. On Linux, returns $XDG_DATA_HOME
     /// if this environment variable is set, $HOME/.local/share otherwise.
     /// @return common applications data directory.
     /// @throws std::runtime_error.
     static std::filesystem::path get_app_data_dir();

     std::string app_data_dir;               ///< path to directory with data used by application (current is default).
     std::string app_dir;                    ///< path to directory with application resources (current is default).
     std::string app_name;                   ///< name of the application.
     RenderApiType render_api;               ///< type of render API used by application.
     ProfileType profile;                    ///< profile used by application.
     ILogWriter::LogLevel log_level;         ///< logging level of the application (default is Info).
     tools::PackageFormat resources_format;  ///< format of resources (default is FlatBuffers).
     tools::PackageFormat scene_format;      ///< format of scene data (default is FlatBuffers).
     bool resources_unpacked;                ///< read unpacked or packed resources (default is false).
};

} // namespace _16nar

#endif // #ifndef _16NAR_GAME_CONFIG_H
