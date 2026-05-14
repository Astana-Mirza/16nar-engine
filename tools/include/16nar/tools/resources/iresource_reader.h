/// @file
/// @brief File with IResourceReader interface definition.
#ifndef _16NAR_TOOLS_IRESOURCE_READER_H
#define _16NAR_TOOLS_IRESOURCE_READER_H

#include <16nar/tools/defs.h>

#include <16nar/tools/misc/shared_buffer_ptr.h>
#include <16nar/tools/misc/static_name.h>

namespace _16nar::tools
{

/// @brief Interface for resource files reading.
class ENGINE_API IResourceReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceReader() = default;

     /// @brief Read resource data to memory buffer.
     /// @param[in] name name of the resource.
     /// @param[in] storage unified storage of resources.
     /// @return data of the resource, nullptr in case of error.
     virtual SharedBufferPtr read( StaticName name, UnifiedStorage& storage ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IRESOURCE_READER_H
