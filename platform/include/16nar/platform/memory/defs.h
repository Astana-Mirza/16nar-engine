/// @file
/// @brief File with basic definitions for the memory component.
#ifndef _16NAR_PLATFORM_MEMORY_DEFS_H
#define _16NAR_PLATFORM_MEMORY_DEFS_H

#include <16nar/platform/defs.h>

#include <memory>

namespace _16nar::memory
{

/// @brief View of buffer of bytes.
struct ByteView
{
     std::byte *data{};  ///< pointer to buffer data.
     std::size_t size{}; ///< size of buffer, in bytes.

     /// @brief Bool conversion operator.
     constexpr explicit operator bool() const noexcept
     {
          return data && size;
     }
};


/// @brief View of immutable buffer of bytes.
struct ConstByteView
{
     const std::byte *data{}; ///< pointer to buffer data.
     std::size_t size{};      ///< size of buffer, in bytes.

     /// @brief Bool conversion operator.
     constexpr explicit operator bool() const noexcept
     {
          return data && size;
     }
};


class MemoryManager;
class IMemoryDomain;
using MemoryManagerPtr = std::shared_ptr< MemoryManager >;

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_DEFS_H
