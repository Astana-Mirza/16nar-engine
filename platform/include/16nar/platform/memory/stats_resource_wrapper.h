/// @file
/// @brief File with StatsResourceWrapper template class definition.
#ifndef _16NAR_PLATFORM_MEMORY_STATS_RESOURCE_WRAPPER_H
#define _16NAR_PLATFORM_MEMORY_STATS_RESOURCE_WRAPPER_H

#include <16nar/platform/memory/memory_usage.h>

namespace _16nar::memory
{

/// @brief Wrapper of the memory resource for usage statistics accounting.
/// @note Different counters of usage information may be inconsistent
/// in case of multi-threaded allocations. Memory resource must have
/// @b get_usage() and @b release() member functions.
/// @tparam Resource type of memory resource.
/// @tparam CounterType type of a single statistics counter.
template < typename Resource, typename CounterType >
class StatsResourceWrapper : public Resource
{
public:
     using Resource::Resource;

     /// @brief Get usage of memory resource.
     /// @note Different counters of usage information may be
     /// inconsistent in case of multi-threaded allocations.
     /// @return usage of memory resource.
     MemoryUsage get_usage() const noexcept
     {
          MemoryUsage usage{};
          usage.bytes_total = bytes_total_;
          usage.bytes_current = bytes_current_;
          usage.alloc_total = alloc_total_;
          usage.alloc_current = alloc_current_;
          return usage;
     }

     /// @brief Release memory resource to default state and reset all usage statistics.
     void release()
     {
          bytes_total_ = 0;
          bytes_current_ = 0;
          alloc_total_ = 0;
          alloc_current_ = 0;
          Resource::release();
     }

protected:
     /// @brief Allocate memory of at least requested size with requested alignment.
     /// @param[in] bytes requested allocation size, in bytes.
     /// @param[in] alignment requested alignment.
     /// @return pointer to allocated memory.
     void *do_allocate( std::size_t bytes, std::size_t alignment ) override
     {
          bytes_total_ += bytes;
          bytes_current_ += bytes;
          ++alloc_total_;
          ++alloc_current_;
          return Resource::do_allocate( bytes, alignment );
     }

     /// @brief Deallocate requested memory.
     /// @param[in] ptr pointer to memory to be deallocated.
     /// @param[in] bytes size of memory to be deallocated.
     /// @param[in] alignment alignment of memory to be deallocated.
     void do_deallocate( void *ptr, std::size_t bytes, std::size_t alignment ) override
     {
          bytes_current_ -= bytes;
          --alloc_current_;
          Resource::do_deallocate( ptr, bytes, alignment );
     }

     /// @brief Check equality of current resource and another one.
     /// @param[in] other other resource.
     /// @return true if resources are equal, false otherwise.
     bool do_is_equal( const std::pmr::memory_resource& other ) const noexcept override
     {
          return this == &other;
     }

private:
     CounterType bytes_total_{};   ///< total count of bytes allocated in the domain.
     CounterType bytes_current_{}; ///< current count of bytes allocated in the domain.
     CounterType alloc_total_{};   ///< total count of allocations in the domain.
     CounterType alloc_current_{}; ///< current count of allocated objects in the domain.
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_STATS_RESOURCE_WRAPPER_H
