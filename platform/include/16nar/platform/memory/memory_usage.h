/// @file
/// @brief File with MemoryUsage structure definition.
#ifndef _16NAR_PLATFORM_MEMORY_MEMORY_USAGE_H
#define _16NAR_PLATFORM_MEMORY_MEMORY_USAGE_H

#include <cstdint>

namespace _16nar::memory
{

/// @brief Information about usage of memory domain.
/// @note Statistics is collected approximately because different allocators
/// may allocate blocks larger than requested size.
struct MemoryUsage
{
     std::uint64_t bytes_total{};       ///< total count of bytes allocated in the domain.
     std::uint64_t bytes_current{};     ///< current count of bytes allocated in the domain.
     std::uint64_t alloc_total{};       ///< total count of allocations in the domain.
     std::uint64_t alloc_current{};     ///< current count of allocated objects in the domain.
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_MEMORY_USAGE_H
