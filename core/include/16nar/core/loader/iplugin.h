/// @file
/// @brief File with IPlugin interface definition.
#ifndef _16NAR_CORE_LOADER_IPLUGIN_H
#define _16NAR_CORE_LOADER_IPLUGIN_H

#include <16nar/core/defs.h>

#include <16nar/core/loader/service_locator.h>

#include <cstdint>

namespace _16nar::loader
{

/// @brief Allocator for plugin creation.
/// @details This structure is used for dependency injection
/// of custom allocation strategy through C-style plugin interface.
struct PluginAllocator
{
     /// @brief allocation function.
     void *( *allocate )( std::uint64_t size, std::uint64_t alignment, void *user_data );

     /// @brief deallocation function.
     void ( *deallocate )( void *ptr, std::uint64_t size, std::uint64_t alignment, void *user_data );

     void *user_data;    ///< private user data to be passed to allocator functions.
};


/// @brief Interface of plugin of the engine.
class IPlugin
{
public:
     /// @brief Check version of the engine and its profile.
     /// @param[in] version version of the engine.
     /// @param[in] assets_version version of assets used by the engine.
     /// @param[in] profile_name name of profile with which the engine is built, ASCIIZ-string.
     /// @return true if version check is successful, false otherwise.
     virtual bool check_version( std::uint32_t version,
          std::uint32_t assets_version, const char *profile_name ) = 0;

     /// @brief Initialize the plugin.
     /// @param[in] locator service locator for Core level components.
     /// @return true if the plugin was initialized successfully, false otherwise.
     virtual bool init( const ServiceLocator *locator ) = 0;

     /// @brief Deinitialize the plugin.
     /// @note This function does not destruct the plugin object and does not release
     /// its memory. It is used only as a lifetime hook.
     virtual void deinit() = 0;

     /// @brief Destroy the plugin via calling its destructor and releasing its memory.
     virtual void destroy() = 0;

protected:
     /// @brief Default destructor.
     /// @details Implementations must destroy themselves via @b destroy() member function.
     ~IPlugin() = default;
};


/// @brief Type of plugin creation function.
/// @details Plugins must declare a function called @b create_16nar_plugin() with
/// this signature. This function is searched in dynamic library of the plugin, so
/// it must be declared with extern "C" linkage.
using PluginCreationFunc = IPlugin *( * )( const PluginAllocator * );

} // namespace _16nar::loader

#endif // #ifndef _16NAR_CORE_LOADER_IPLUGIN_H
