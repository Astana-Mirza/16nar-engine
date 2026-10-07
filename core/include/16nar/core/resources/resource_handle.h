/// @file
/// @brief File with ResourceHandle class definition.
#ifndef _16NAR_CORE_RESOURCES_RESOURCE_HANDLER_H
#define _16NAR_CORE_RESOURCES_RESOURCE_HANDLER_H

#include <16nar/core/resources/defs.h>

#include <16nar/platform/execution/defs.h>

namespace _16nar::resources
{

/// @brief Resource handle which should be used by resource consumers.
class NARENGINE_CORE_API ResourceHandle
{
public:
     friend class IResourceManager;

     /// @brief Default constructor.
     ResourceHandle() noexcept;

     /// @brief Destructor.
     ~ResourceHandle();

     /// @brief Copy constructor.
     /// @param other other object.
     ResourceHandle( const ResourceHandle& other );

     /// @brief Move constructor.
     /// @param other other object.
     ResourceHandle( ResourceHandle&& other );

     /// @brief Copy assignment operator.
     /// @param rhs right hand operand.
     /// @return current object.
     ResourceHandle& operator=( const ResourceHandle& rhs );

     /// @brief Move assignment operator.
     /// @param rhs right hand operand.
     /// @return current object.
     ResourceHandle& operator=( ResourceHandle&& rhs );

     /// @brief Bool cast operator.
     /// @return true if the handle is valid, false otherwise.
     explicit operator bool() const noexcept
     {
          return id_ && manager_;
     }

     /// @brief Get resource identifier.
     /// @return resource identifier.
     inline ResID get_id() const noexcept
     {
          return id_;
     }

     /// @brief Get resource manager.
     /// @return resource manager.
     inline IResourceManager *get_manager() const noexcept
     {
          return manager_;
     }

     /// @brief Load the resource.
     /// @details This function schedules load task for execution. @ref wait_load function
     /// must be used in order to use the final loaded resource.
     /// @param[in] mode mode of load task execution.
     /// @return true on success, false otherwise.
     bool load( execution::Mode mode );

     /// @brief Wait for the resource preload.
     /// @return true on success, false otherwise.
     bool wait_preload();

     /// @brief Wait for the resource load.
     /// @return true on success, false otherwise.
     bool wait_load();

protected:
     /// @brief Constructor which should be used only by resource managers.
     /// @details This constructor does not increment the reference counter,
     /// so it should be done in corresponding resource manager.
     /// @param[in] manager resource manager owning the resource.
     /// @param[in] id identifier of the resource.
     ResourceHandle( IResourceManager *manager, ResID id ) noexcept;

private:
     IResourceManager *manager_;   ///< resource manager.
     ResID id_;                    ///< identifier of the resource.
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_RESOURCE_HANDLER_H
