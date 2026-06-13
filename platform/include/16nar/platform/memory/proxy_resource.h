/// @file
/// @brief File with ProxyResource class definition.
#ifndef _16NAR_PLATFORM_MEMORY_PROXY_RESOURCE_H
#define _16NAR_PLATFORM_MEMORY_PROXY_RESOURCE_H

#include <16nar/platform/defs.h>

#include <16nar/platform/memory/memory_usage.h>

#include <memory_resource>
#include <atomic>

namespace _16nar::memory
{

/// @brief Memory resource which does not hold its own memory but
/// acts like a proxy to another resource.
/// @details Proxy resource has its own usage statistics, so it can be used
/// to gather usage information about allocations of some kind but actually
/// using memory of another resource at the same time.
/// @note Different counters of usage information may be inconsistent
/// in case of multi-threaded allocations.
class NARENGINE_PLATFORM_API ProxyResource : public std::pmr::memory_resource
{
public:
     /// @brief Constructor.
     /// @param[in] resource external memory resource to be proxied.
     ProxyResource( std::pmr::memory_resource *resource );

     /// @brief Get usage of memory resource.
     /// @note Different counters of usage information may be
     /// inconsistent in case of multi-threaded allocations.
     /// @return usage of memory resource.
     MemoryUsage get_usage() const noexcept;

     /// @brief Reset statistics, but do not release any memory of proxied resource.
     void release();

protected:
     /// @brief Allocate memory of at least requested size with requested alignment.
     /// @param[in] bytes requested allocation size, in bytes.
     /// @param[in] alignment requested alignment.
     /// @return pointer to allocated memory.
     void *do_allocate( std::size_t bytes, std::size_t alignment ) override;

     /// @brief Deallocate requested memory.
     /// @param[in] ptr pointer to memory to be deallocated.
     /// @param[in] bytes size of memory to be deallocated.
     /// @param[in] alignment alignment of memory to be deallocated.
     void do_deallocate( void *ptr, std::size_t bytes, std::size_t alignment ) override;

     /// @brief Check equality of current resource and another one.
     /// @param[in] other other resource.
     /// @return true if resources are equal, false otherwise.
     bool do_is_equal( const std::pmr::memory_resource& other ) const noexcept override;

private:
     std::pmr::memory_resource& resource_;   ///< external memory resource.
     std::atomic_uint64_t bytes_total_;      ///< total count of bytes allocated in the domain.
     std::atomic_uint64_t bytes_current_;    ///< current count of bytes allocated in the domain.
     std::atomic_uint64_t alloc_total_;      ///< total count of allocations in the domain.
     std::atomic_uint64_t alloc_current_;    ///< current count of allocated objects in the domain.
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_PROXY_RESOURCE_H
