/// @file
/// @brief File with ConfigManager class definition.
#ifndef _16NAR_CORE_CONFIG_CONFIG_MANAGER_H
#define _16NAR_CORE_CONFIG_CONFIG_MANAGER_H

#include <16nar/core/defs.h>
#include <16nar/platform/strings/name_table.h>

#include <string>
#include <variant>
#include <unordered_map>
#include <memory_resource>
#include <filesystem>

namespace _16nar::config
{

/// @brief Manager of both engine and application configuration.
/// @details Configuration parameters are stored within sections.
/// Each section has unique name and each parameter has unique name
/// inside its section. Configuration is stored in INI file.
/// In order to use a configuration parameter (or read it from file),
/// one at first needs to register it in the manager.
class NARENGINE_CORE_API ConfigManager
{
public:
     /// @brief Type of configuration value.
     using ConfigValue = std::variant<
          std::uint64_t,
          std::int64_t,
          double,
          bool,
          std::pmr::string
     >;

     /// @brief Type of configuration parameter.
     enum class ConfigType
     {
          Uint,     ///< Unsigned integer, default value is 0.
          Int,      ///< Integer, default value is 0.
          Float,    ///< Floating point number, default value is 0.0.
          Bool,     ///< Boolean, default value is false.
          String    ///< String, default value is empty string.
     };

     /// @brief Flags of configuration parameter behavior.
     enum ConfigFlags : std::uint8_t
     {
          None      = 0,      ///< no flags.
          Dynamic   = 1 << 0, ///< parameter's value can be changed.
          Hidden    = 1 << 1  ///< parameter is not saved to configuration (only for dynamic parameters).
     };

     /// @brief Parameter of a configuration.
     struct ConfigParam
     {
          ConfigValue value{};                         ///< current value of a configuration parameter.
          ConfigType type{ ConfigType::Uint };         ///< type of configuration parameter.
          ConfigFlags flags{ ConfigFlags::Dynamic };   ///< flags of configuration parameter behavior.
     };

     /// @brief Constructor.
     /// @param[in] names table of parameter and section names.
     /// @param[in] resource memory resource for configuration data allocations.
     ConfigManager( strings::NameTable& names, std::pmr::memory_resource& resource );

     /// @brief Register configuration parameter.
     /// @details Parameter is registered with default value of its type.
     /// Parameter cannot be added if it has duplicate name within the same section.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @param[in] type type of configuration parameter.
     /// @param[in] flags flags of configuration parameter.
     /// @return true if parameter is added, false otherwise.
     bool add_parameter( std::string_view section, std::string_view name,
          ConfigType type, ConfigFlags flags = ConfigFlags::Dynamic );

     /// @brief Load configuration from INI file.
     /// @details Configuration must be locked at the moment of reading, so
     /// that static parameters cannot be overwritten. This function will return
     /// false if the configuration is not locked.
     /// If a hidden dynamic parameter is set in the file, it will be marked as
     /// non-hidden in order to save it to file in the future.
     /// @param[in] path path to the configuration file.
     /// @return true on success, false otherwise.
     bool load_from_file( const std::filesystem::path& path );

     /// @brief Save configuration to INI file.
     /// @details Only dynamic non-hidden parameters are saved.
     /// @param[in] path path to the configuration file.
     /// @return true on success, false otherwise.
     bool save_to_file( const std::filesystem::path& path ) const;

     /// @brief Protect configuration from static parameters changing.
     /// @details After calling this function, configuration cannot be unlocked.
     void lock_config() noexcept;

     /// @brief Get unsigned integer configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @return value of the parameter, default value if it is not registered.
     std::uint64_t get_uint( strings::StaticName section, strings::StaticName name ) const noexcept;

     /// @brief Get signed integer configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @return value of the parameter, default value if it is not registered.
     std::int64_t get_int( strings::StaticName section, strings::StaticName name ) const noexcept;

     /// @brief Get floating point configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @return value of the parameter, default value if it is not registered.
     double get_float( strings::StaticName section, strings::StaticName name ) const noexcept;

     /// @brief Get boolean configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @return value of the parameter, default value if it is not registered.
     bool get_bool( strings::StaticName section, strings::StaticName name ) const noexcept;

     /// @brief Get string configuration value.
     /// @details Returned value is valid until the parameter value is overwritten.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @return value of the parameter, default value if it is not registered.
     std::string_view get_string( strings::StaticName section, strings::StaticName name ) const noexcept;

     /// @brief Set unsigned integer configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @param[in] value value of the parameter.
     /// @return true if value is successfully set, false if the parameter is not registered.
     bool set_uint( strings::StaticName section, strings::StaticName name, std::uint64_t value );

     /// @brief Set signed integer configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @param[in] value value of the parameter.
     /// @return true if value is successfully set, false if the parameter is not registered.
     bool set_int( strings::StaticName section, strings::StaticName name, std::int64_t value );

     /// @brief Set floating point configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @param[in] value value of the parameter.
     /// @return true if value is successfully set, false if the parameter is not registered.
     bool set_float( strings::StaticName section, strings::StaticName name, double value );

     /// @brief Set boolean configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @param[in] value value of the parameter.
     /// @return true if value is successfully set, false if the parameter is not registered.
     bool set_bool( strings::StaticName section, strings::StaticName name, bool value );

     /// @brief Set string configuration value.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @param[in] value value of the parameter.
     /// @return true if value is successfully set, false if the parameter is not registered.
     bool set_string( strings::StaticName section, strings::StaticName name, std::string_view value );

private:
     ConfigManager( const ConfigManager&& ) = delete;
     ConfigManager& operator=( const ConfigManager& ) = delete;

     /// @brief Configuration entries of a single section.
     using Section = std::pmr::unordered_map< strings::StaticName, ConfigParam >;

     /// @brief Callback called when configuration parameter is parsed.
     /// @param[in] user user data passed to callback.
     /// @param[in] section name of current section.
     /// @param[in] name name of current parameter.
     /// @param[in] value value of current parameter.
     /// @return 1 on success, 0 on error.
     static int parse_callback( void *user, const char *section, const char *name, const char *value );

     /// @brief Get constant configuration parameter.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @return pointer to configuration parameter if it is registered, nullptr otherwise.
     const ConfigParam *get_param( strings::StaticName section, strings::StaticName name ) const noexcept;

     /// @brief Get configuration parameter.
     /// @param[in] section name of configuration section.
     /// @param[in] name name of configuration parameter within the section.
     /// @return pointer to configuration parameter if it is registered, nullptr otherwise.
     ConfigParam *get_param( strings::StaticName section, strings::StaticName name ) noexcept;

private:
     std::pmr::unordered_map< strings::StaticName, Section > config_; ///< configuration parameters.
     strings::NameTable& names_;                  ///< table of parameter and section names.
     std::pmr::memory_resource& resource_;        ///< memory resource for configuration data allocations.
     bool locked_;                                ///< flag of protection against static paramters overwrite.
};

} // namespace _16nar::config

#endif // #ifndef _16NAR_CORE_CONFIG_CONFIG_MANAGER_H
