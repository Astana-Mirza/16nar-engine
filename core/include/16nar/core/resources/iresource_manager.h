/// @file
/// @brief File with IResourceManager interface definition.
#ifndef _16NAR_CORE_RESOURCES_IRESOURCE_MANAGER_H
#define _16NAR_CORE_RESOURCES_IRESOURCE_MANAGER_H

#include <16nar/core/resources/defs.h>
#include <16nar/core/resources/resource_handle.h>

#include <16nar/platform/memory/shared_buffer_ptr.h>
#include <16nar/platform/strings/static_name.h>
#include <16nar/platform/execution/defs.h>

namespace _16nar::resources
{

/// @brief Interface of a manager which holds resources of same type.
class IResourceManager
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceManager() = default;

     /// @brief Load resource to manager.
     /// @details This function schedules load task for execution. @ref ResourceHandle::wait_load
     /// function must be used in order to use the final loaded resource.
     /// @param[in] name name of the resource.
     /// @param[in] mode mode of load task execution.
     /// @return resource handle, empty handle in case of error.
     virtual ResourceHandle load( strings::StaticName name, execution::Mode mode ) = 0;

     /// @brief Preload resource to manager.
     /// @details This function schedules preload task for execution. @ref ResourceHandle::wait_preload
     /// function must be used in order to use the final preloaded resource.
     /// @param[in] name name of the resource.
     /// @param[in] mode mode of preload task execution.
     /// @return resource handle, empty handle in case of error.
     virtual ResourceHandle preload( strings::StaticName name, execution::Mode mode ) = 0;

protected:
     friend class ResourceHandle;

     /// @brief Load resource by its identifier.
     /// @details This function is usually called to load a resource
     /// which has already been preloaded earlier.
     /// @param[in] res_id identifier of the resource.
     /// @param[in] mode mode of load task execution.
     /// @return true on success, false on failure.
     virtual bool load_by_id( ResID res_id, execution::Mode mode ) = 0;

     /// @brief Wait for the resource preload.
     /// @param[in] res_id identifier of the resource.
     /// @return true on success, false otherwise.
     virtual bool wait_preload( ResID res_id ) = 0;

     /// @brief Wait for the resource load.
     /// @param[in] res_id identifier of the resource.
     /// @return true on success, false otherwise.
     virtual bool wait_load( ResID res_id ) = 0;

     /// @brief Increment reference counter of the resource.
     /// @param[in] res_id identifier of the resource.
     virtual void inc_ref( ResID res_id ) = 0;

     /// @brief Decrement reference counter of the resource.
     /// @details When the reference count becomes zero, the resource generally should be released.
     /// @param[in] res_id identifier of the resource.
     virtual void dec_ref( ResID res_id ) = 0;

     /// @brief Create resource handle associated with this manager.
     /// @param[in] res_id identifier of the resource.
     /// @return handle of the resource.
     inline ResourceHandle create_handle( ResID res_id ) noexcept
     {
          return ResourceHandle{ this, res_id };
     }
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_IRESOURCE_MANAGER_H
