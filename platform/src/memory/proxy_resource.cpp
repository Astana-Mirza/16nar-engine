#include <16nar/platform/memory/proxy_resource.h>

namespace _16nar::memory
{

ProxyResource::ProxyResource( std::pmr::memory_resource *resource ):
     resource_{ *resource }, bytes_total_{}, bytes_current_{},
     alloc_total_{}, alloc_current_{}
{}


MemoryUsage ProxyResource::get_usage() const noexcept
{
     MemoryUsage usage{};
     usage.bytes_total = bytes_total_;
     usage.bytes_current = bytes_current_;
     usage.alloc_total = alloc_total_;
     usage.alloc_current = alloc_current_;
     return usage;
}


void ProxyResource::release()
{
     bytes_total_ = 0;
     bytes_current_ = 0;
     alloc_total_ = 0;
     alloc_current_ = 0;
}


void *ProxyResource::do_allocate( std::size_t bytes, std::size_t alignment )
{
     bytes_total_ += bytes;
     bytes_current_ += bytes;
     ++alloc_total_;
     ++alloc_current_;
     return resource_.allocate( bytes, alignment );
}


void ProxyResource::do_deallocate( void *ptr, std::size_t bytes, std::size_t alignment )
{
     bytes_current_ -= bytes;
     --alloc_current_;
     resource_.deallocate( ptr, bytes, alignment );
}


bool ProxyResource::do_is_equal( const std::pmr::memory_resource& other ) const noexcept
{
     return this == &other;
}

} // namespace _16nar::memory
