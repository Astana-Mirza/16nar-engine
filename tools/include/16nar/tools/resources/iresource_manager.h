/// @file
/// @brief File with IResourceManager interface definition.
#ifndef _16NAR_TOOLS_IRESOURCE_MANAGER_H
#define _16NAR_TOOLS_IRESOURCE_MANAGER_H

#include <16nar/tools/defs.h>

namespace _16nar::tools
{

/// @brief Interface of a manager which holds resources of same type.
class ENGINE_API IResourceManager
{
public:
     /// @brief Virtual default destructor.
     virtual ~IResourceManager() = default;

     /// @brief Load resource to manager.
     /// @param[in] name name of the resource.
     /// @return resource identifier, 0 in case of error.
     virtual ResID load( StaticName name ) = 0;

     /// @brief Unload resource from manager and free its memory.
     /// @param[in] id resource identifier.
     virtual void unload( ResID id ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IRESOURCE_MANAGER_H
