/// @file
/// @brief File with IResourceProcessor interface definition.
#ifndef _16NAR_CORE_RESOURCES_IRESOURCE_PROCESSOR_H
#define _16NAR_CORE_RESOURCES_IRESOURCE_PROCESSOR_H

#include <16nar/core/resources/defs.h>

#include <16nar/platform/memory/shared_buffer_ptr.h>

namespace _16nar::resources
{

/// @brief Interface for processing the resource during its load.
class IResourceProcessor
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceProcessor() = default;

     /// @brief Start resource processing.
     /// @param metadata resource metadata buffer.
     /// @return current resource identifier.
     virtual ResID start( memory::ConstByteView metadata ) = 0;

     /// @brief Process part of resource data.
     /// @param[in] res_id resource identifier.
     /// @param[in] data_id resource data part identifier.
     /// @param[in] data data of the resource.
     /// @return true on success, false otherwise.
     virtual bool process_data( ResID res_id, DataID data_id, memory::SharedBufferPtr data ) = 0;

     /// @brief Finish resource processing.
     /// @param[in] res_id resource identifier.
     /// @return true on success, false otherwise.
     virtual bool finish( ResID res_id ) = 0;
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_IRESOURCE_PROCESSOR_H
