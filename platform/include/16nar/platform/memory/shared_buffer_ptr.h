/// @file
/// @brief File with SharedBufferPtr class definition.
#ifndef _16NAR_PLATFORM_MEMORY_SHARED_BUFFER_PTR_H
#define _16NAR_PLATFORM_MEMORY_SHARED_BUFFER_PTR_H

#include <16nar/platform/memory/defs.h>

#include <memory_resource>

namespace _16nar::memory
{

/// @brief Pointer to shared data buffer.
/// @details Thread-unsafe pointer to memory block with reference counting.
/// Alignment of the memory block is equal to alignas( std::max_align_t ).
/// The control block is located in the beginning of the memory block, so the size
/// of control block is added to size of requested memory during allocation.
class NARENGINE_PLATFORM_API SharedBufferPtr
{
public:
     /// @brief Allocate memory buffer with maximum possible alignment.
     /// @details Size of control block is added to size of requested memory during allocation.
     /// @throws std::bad_alloc if unable to allocate memory.
     /// @param[in] resource memory resource for the allocation.
     /// @param[in] size size of the buffer to be allocated, not including control block size.
     /// @return pointer to allocated buffer.
     static SharedBufferPtr allocate( std::pmr::memory_resource& resource, std::size_t size );

     /// @brief Default constructor.
     SharedBufferPtr() noexcept;

     /// @brief Copy constructor.
     /// @param[in] other object to be copied.
     SharedBufferPtr( const SharedBufferPtr& other ) noexcept;

     /// @brief Move constructor.
     /// @param[in] other object to be moved.
     SharedBufferPtr( SharedBufferPtr&& other ) noexcept;

     /// @brief Copy assignment operator.
     /// @param rhs right operand.
     /// @return Current object.
     SharedBufferPtr& operator=( const SharedBufferPtr& rhs ) noexcept;

     /// @brief Move assignment operator.
     /// @param rhs right operand.
     /// @return Current object.
     SharedBufferPtr& operator=( SharedBufferPtr&& rhs ) noexcept;

     /// @brief Destructor.
     ~SharedBufferPtr();

     /// @brief Bool cast operator.
     /// @return true if pointer is not null, false otherwise.
     explicit operator bool() const noexcept;

     /// @brief Reset the pointer.
     /// @details After the reset, the pointer's state will be as if it was default constructed.
     void reset();

     /// @brief Get count of pointers to the same buffer, including the current one.
     /// @return buffer use count or zero if the pointer is null.
     std::size_t use_count() const noexcept;

     /// @brief Get view of buffer data.
     /// @return view of buffer data.
     ByteView get_view() const noexcept;

     /// @brief Get immutable view of buffer data.
     /// @return immutable view of buffer data.
     ConstByteView get_const_view() const noexcept;

private:
     /// @brief Block with reference count.
     struct alignas( std::max_align_t ) ControlBlock
     {
          std::size_t size{};      ///< size of the buffer not including size of the control block.
          std::size_t refs{};      ///< count of references to the buffer.
     };

     std::pmr::memory_resource *resource_;   ///< memory resource used to allocate the buffer.
     std::byte *data_;                       ///< pointer to raw buffer beginning with control block.
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_SHARED_BUFFER_PTR_H
