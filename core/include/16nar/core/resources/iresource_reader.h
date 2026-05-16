/// @file
/// @brief File with IResourceReader interface definition.
#ifndef _16NAR_CORE_RESOURCES_IRESOURCE_READER_H
#define _16NAR_CORE_RESOURCES_IRESOURCE_READER_H

#include <16nar/core/resources/defs.h>

#include <16nar/platform/memory/shared_buffer_ptr.h>
#include <16nar/platform/strings/static_name.h>

namespace _16nar::resources
{

/// @brief Interface for resource files reading.
class IResourceReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceReader() = default;

     /// @brief Read resource data to memory buffer.
     /// @param[in] name name of the resource.
     /// @param[in] storage unified storage of resources.
     /// @return data of the resource, nullptr in case of error.
     virtual memory::SharedBufferPtr read( strings::StaticName name, UnifiedStorage& storage ) = 0;
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_IRESOURCE_READER_H
