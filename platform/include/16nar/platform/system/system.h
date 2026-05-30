/// @file
/// @brief Header file with system functions declaration.
#ifndef _16NAR_PLATFORM_SYSTEM_SYSTEM_H
#define _16NAR_PLATFORM_SYSTEM_SYSTEM_H

#include <16nar/platform/defs.h>

namespace _16nar::system
{

/// @brief Initialize system functions of the engine.
/// @details This function must be called in main thread before any other
/// system functions. There is no effect if this function is called again
/// after initialization.
/// @return true on success, false otherwise.
bool NARENGINE_PLATFORM_API init() noexcept;

/// @brief Deinitialize system functions of the engine.
/// @details This function must be called in main thread before application
/// termination after all resources are released. After it is called,
/// engine cannot use its functions anymore. There is no effect if this
/// function is called before the initialization.
void NARENGINE_PLATFORM_API deinit() noexcept;

/// @brief Process all input events that have occured since last process.
/// @details This function must be called only from main thread.
/// It calls all needed callbacks registered for occured events.
void NARENGINE_PLATFORM_API process_input_events() noexcept;

/// @brief Put current thread to sleep and wait for input events.
/// @details This function must be called only from main thread.
/// It calls all needed callbacks registered for occured events.
/// @param[in] timeout maximum wait time, 0 for unlimited time.
void NARENGINE_PLATFORM_API wait_input_events( double timeout ) noexcept;

/// @brief Post empty event to stop main thread which is waiting
/// with @ref wait_input_events.
/// @details This function may be called from any thread.
void NARENGINE_PLATFORM_API post_empty_event() noexcept;

/// @brief Get time since system functions initialization, in seconds.
/// @details The time is monotonic. This function may be called from
/// any thread, but it is not synchronized with @ref set_time.
/// @return time since system functions initialization, in seconds.
double NARENGINE_PLATFORM_API get_time() noexcept;

/// @brief Set current monotonic time value.
/// @details This function may be called from
/// any thread, but it is not synchronized with @ref get_time.
/// @param[in] value new current time, in seconds.
void NARENGINE_PLATFORM_API set_time( double value ) noexcept;

} // namespace _16nar::system


#endif // #ifndef _16NAR_PLATFORM_SYSTEM_SYSTEM_H
