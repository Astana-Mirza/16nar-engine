/// @file
/// @brief File with IResourceLoader interface definition.
#ifndef _16NAR_TOOLS_IRESOURCE_LOADER_H
#define _16NAR_TOOLS_IRESOURCE_LOADER_H

#include <16nar/tools/defs.h>

namespace _16nar::tools
{

/// @brief Main interface of resource loading.
/// @details Resource loader manages the load process of resource metadata and data
/// and uses @b IResourceProcessor to perform needed operations with the data.
class ENGINE_API IResourceLoader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceLoader() = default;

     /// @brief Load resource and pass it to consuming processor.
     /// @param[in] name name of the resource.
     /// @param[in] processor object which processes the loaded resource data.
     virtual void load( StaticName name, IResourceProcessor& processor ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IRESOURCE_LOADER_H
