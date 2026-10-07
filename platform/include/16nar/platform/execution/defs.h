/// @file
/// @brief File with basic definitions for the execution component.
#ifndef _16NAR_PLATFORM_EXECUTION_DEFS_H
#define _16NAR_PLATFORM_EXECUTION_DEFS_H

#include <cstdint>

namespace _16nar::execution
{

/// @brief Mode of execution.
enum class Mode : std::uint8_t
{
     Sync      = 0, ///< synchronous execution.
     Async     = 1  ///< asynchronous execution.
};

} // namespace _16nar::execution

#endif // #ifndef _16NAR_PLATFORM_EXECUTION_DEFS_H
