/// @file
/// @brief File with IResourceLoader interface definition.
#ifndef _16NAR_CORE_RESOURCES_IRESOURCE_LOADER_H
#define _16NAR_CORE_RESOURCES_IRESOURCE_LOADER_H

#include <16nar/core/resources/defs.h>

#include <16nar/platform/memory/shared_buffer_ptr.h>
#include <16nar/platform/strings/static_name.h>

namespace _16nar::resources
{

/// @brief Main interface of resource loading.
/// @details Resource loader manages the load process of resource data
/// and uses @b IResourceProcessor to perform needed operations with the data.
class IResourceLoader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceLoader() = default;

     /// @brief Load resource and pass it to consuming processor.
     /// @param[in] res_id current resource identifier acting as its unique key.
     /// @param[in] metadata resource metadata buffer.
     /// @param[in] processor object which processes the loaded resource data.
     /// @return true on success, false otherwise.
     virtual bool load( ResID res_id, memory::ConstByteView metadata, IResourceProcessor& processor ) = 0;
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_IRESOURCE_LOADER_H
