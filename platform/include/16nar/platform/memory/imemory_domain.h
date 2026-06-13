/// @file
/// @brief File with IMemoryDomain interface definition.
#ifndef _16NAR_PLATFORM_MEMORY_IMEMORY_DOMAIN_H
#define _16NAR_PLATFORM_MEMORY_IMEMORY_DOMAIN_H

#include <16nar/platform/memory/defs.h>

#include <16nar/platform/memory/memory_usage.h>

#include <memory_resource>

namespace _16nar::memory
{

/// @brief Abstract memory domain having its own memory resource.
/// @note All derived classes must have upstream memory resource
/// as first parameter of constructor, because @ref MemoryManager
/// passes parent domain resource there, when creating the domain.
class IMemoryDomain
{
public:
     /// @brief Constructor.
     /// @param[in] upstream upstream memory resource.
     IMemoryDomain( std::pmr::memory_resource *upstream )
     {
          ( void ) upstream;
     }

     /// @brief Virtual default destructor.
     virtual ~IMemoryDomain() = default;

     /// @brief Get memory resource of the domain.
     /// @return memory resource of the domain.
     virtual std::pmr::memory_resource& get_resource() = 0; 

     /// @brief Get information about usage of the memory domain.
     /// @return usage of the memory domain.
     virtual MemoryUsage get_usage() const = 0;

protected:
     /// @brief Reset the memory domain and release all the memory, if possible.
     /// @details Children domains, if any, must always be reset before the parent domain.
     virtual void reset() = 0;

     friend class MemoryManager;
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_IMEMORY_DOMAIN_H
