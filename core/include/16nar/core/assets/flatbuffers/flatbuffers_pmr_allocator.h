/// @file
/// @brief File with FlatBuffersPmrAllocator class definition.
#ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_PMR_ALLOCATOR_H
#define _16NAR_CORE_ASSETS_FLATBUFFERS_PMR_ALLOCATOR_H

#include <16nar/core/assets/defs.h>

#include <flatbuffers/allocator.h>

#include <memory_resource>

namespace _16nar::assets
{

/// @brief Allocator for flatbuffers which uses polymorphic memory resource.
class NARENGINE_ASSETS_FB_API FlatBuffersPmrAllocator : public flatbuffers::Allocator
{
public:
     /// @brief Default constructor.
     FlatBuffersPmrAllocator() noexcept;

     /// @brief Constructor.
     /// @param[in] resource memory resource for allocations.
     explicit FlatBuffersPmrAllocator( std::pmr::memory_resource& resource ) noexcept;

     /// @brief Allocate buffer of bytes with given size.
     /// @throws std::bad_alloc depending on underlying memory resource.
     /// @param[in] size size of the buffer to be allocated, in bytes.
     /// @return pointer to allocated buffer.
     std::uint8_t *allocate( std::size_t size ) override;

     /// @brief Deallocate buffer previously allocated by this allocator.
     /// @param[in] data buffer pointer.
     /// @param[in] size size of the buffer, in bytes.
     void deallocate( std::uint8_t *data, std::size_t size ) override;

private:
     std::pmr::memory_resource *resource_;   ///< underlying memory resource for allocaitons.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_PMR_ALLOCATOR_H
