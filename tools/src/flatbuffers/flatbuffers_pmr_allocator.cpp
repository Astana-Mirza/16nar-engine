#include <16nar/tools/flatbuffers/flatbuffers_pmr_allocator.h>

#include <cassert>

namespace _16nar::tools
{

FlatBuffersPmrAllocator::FlatBuffersPmrAllocator() noexcept:
     resource_{}
{
     resource_ = std::pmr::get_default_resource();
     assert( resource_ );
}


FlatBuffersPmrAllocator::FlatBuffersPmrAllocator( std::pmr::memory_resource& resource ) noexcept:
     resource_{ &resource }
{}


std::uint8_t *FlatBuffersPmrAllocator::allocate( std::size_t size )
{
     assert( resource_ );
     return static_cast< std::uint8_t * >( resource_->allocate( size ) );
}


void FlatBuffersPmrAllocator::deallocate( std::uint8_t *data, std::size_t size )
{
     assert( resource_ );
     resource_->deallocate( data, size );
}

} // namespace _16nar::tools
