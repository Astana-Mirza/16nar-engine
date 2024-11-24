/// @file
/// @brief File with IResourceManager interface definition.
#ifndef _16NAR_IRESOURCE_MANAGER_H
#define _16NAR_IRESOURCE_MANAGER_H

#include <16nar/16nardefs.h>

#include <any>

namespace _16nar
{

/// @brief Interface for various resource loading.
class ENGINE_API IResourceManager
{
public:
     /// @brief Default virtual destructor.
     virtual ~IResourceManager() = default;

     /// @brief Request loading a resource.
     /// @param[in] params parameters of loading the resource.
     /// @return ID of loaded resource.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException, ExceededIdException.
     virtual ResID load( const std::any& params ) = 0;

     /// @brief Unload resource and free memory.
     /// @param[in] id ID of resource to be unloaded.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException.
     virtual void unload( ResID id ) = 0;

     /// @brief Unload all resources.
     virtual void clear() = 0;

     /// @brief Get handler of the resource.
     /// @details Throws ResourceException if resource with given ID does not exist.
     /// @param[in] id ID of resource.
     /// @throws ResourceException if resource is not found.
     /// @return handler of the resource.
     virtual std::any get_handler( ResID id ) const = 0;

     /// @brief Process all requests in load queue.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException, ExceededIdException.
     virtual void process_load_queue() {}

     /// @brief Process all requests in unload queue.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException.
     virtual void process_unload_queue() {}

     /// @brief Swap queues and prepare for new frame.
     virtual void end_frame() {}
};

} // namespace _16nar

#endif // #ifndef _16NAR_IRESOURCE_MANAGER_H
