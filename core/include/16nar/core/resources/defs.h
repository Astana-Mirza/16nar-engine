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


class UnifiedStorage;
class IResourceLoader;
class IResourceProcessor;
class IResourceReader;
class IResourceManager;

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_DEFS_H
