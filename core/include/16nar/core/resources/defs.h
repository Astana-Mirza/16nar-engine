/// @file
/// @brief File with basic definitions for the resources component.
#ifndef _16NAR_CORE_RESOURCES_DEFS_H
#define _16NAR_CORE_RESOURCES_DEFS_H

#include <16nar/core/defs.h>

#include <cstdint>

namespace _16nar::resources
{

/// @brief Resource descriptor.
using ResID = std::uint32_t;

/// @brief Identifier of resource data part.
using DataID = std::uint32_t;

/// @brief State of resource lifecycle.
enum class ResourceState : std::uint8_t
{
     Unloaded     = 0,   ///< Default state.
     Preloading   = 1,   ///< Resource is in a process of preload.
     Preloaded    = 2,   ///< Resource is preloaded and ready for load.
     Loading      = 3,   ///< Resource is in a process of load.
     Loaded       = 4,   ///< Resource is loaded and ready for use.
     Unpreloading = 5,   ///< Resource is in a process of unload from preloaded state.
     Unloading    = 6    ///< Resource is in a process of unload from loaded state.
};

class UnifiedStorage;
class IResourceLoader;
class IResourceProcessor;
class IResourceReader;
class IResourceManager;

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_DEFS_H
