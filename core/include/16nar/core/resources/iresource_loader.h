/// @file
/// @brief File with IResourceLoader interface definition.
#ifndef _16NAR_CORE_RESOURCES_IRESOURCE_LOADER_H
#define _16NAR_CORE_RESOURCES_IRESOURCE_LOADER_H

#include <16nar/core/resources/defs.h>

#include <16nar/platform/strings/static_name.h>

namespace _16nar::resources
{

/// @brief Main interface of resource loading.
/// @details Resource loader manages the load process of resource metadata and data
/// and uses @b IResourceProcessor to perform needed operations with the data.
class NARENGINE_CORE_API IResourceLoader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceLoader() = default;

     /// @brief Load resource and pass it to consuming processor.
     /// @param[in] name name of the resource.
     /// @param[in] processor object which processes the loaded resource data.
     virtual void load( strings::StaticName name, IResourceProcessor& processor ) = 0;
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_IRESOURCE_LOADER_H
