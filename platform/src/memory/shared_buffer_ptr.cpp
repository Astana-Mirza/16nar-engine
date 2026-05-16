#include <16nar/platform/memory/shared_buffer_ptr.h>

namespace _16nar::memory
{

SharedBufferPtr SharedBufferPtr::allocate( std::pmr::memory_resource& resource, std::size_t size )
{
     SharedBufferPtr ptr{};
     ptr.resource_ = &resource;
     ptr.data_ = static_cast< std::byte * >( resource.allocate( sizeof( ControlBlock ) + size ) );
     new ( ptr.data_ ) ControlBlock{ size, 1 };
     return ptr;
}


SharedBufferPtr::SharedBufferPtr() noexcept:
     resource_{}, data_{}
{}


SharedBufferPtr::SharedBufferPtr( const SharedBufferPtr& other ) noexcept:
     resource_{ other.resource_ }, data_{ other.data_ }
{
     if ( data_ )
     {
          ++reinterpret_cast< ControlBlock * >( data_ )->refs;
     }
}


SharedBufferPtr::SharedBufferPtr( SharedBufferPtr&& other ) noexcept:
     resource_{ other.resource_ }, data_{ other.data_ }
{
     other.resource_ = nullptr;
     other.data_ = nullptr;
}


SharedBufferPtr& SharedBufferPtr::operator=( const SharedBufferPtr& rhs ) noexcept
{
     if ( &rhs != this )
     {
          reset();
          resource_ = rhs.resource_;
          data_ = rhs.data_;
          if ( data_ )
          {
               ++reinterpret_cast< ControlBlock * >( data_ )->refs;
          }
     }
     return *this;
}


SharedBufferPtr& SharedBufferPtr::operator=( SharedBufferPtr&& rhs ) noexcept
{
     if ( &rhs != this )
     {
          reset();
          resource_ = rhs.resource_;
          data_ = rhs.data_;
          rhs.resource_ = nullptr;
          rhs.data_ = nullptr;
     }
     return *this;
}


SharedBufferPtr::~SharedBufferPtr()
{
     reset();
}


SharedBufferPtr::operator bool() const noexcept
{
     return data_;
}


void SharedBufferPtr::reset()
{
     if ( !data_ )
     {
          return;
     }

     auto *block = reinterpret_cast< ControlBlock * >( data_ );
     if ( --block->refs == 0 )
     {
          resource_->deallocate( data_, block->size + sizeof( ControlBlock ) );
     }
     resource_ = nullptr;
     data_ = nullptr;
}


std::size_t SharedBufferPtr::use_count() const noexcept
{
     return data_ ? reinterpret_cast< ControlBlock * >( data_ )->refs : 0;
}


ByteView SharedBufferPtr::get_view() const noexcept
{
     return data_ ? ByteView{ data_ + sizeof( ControlBlock ),
          reinterpret_cast< ControlBlock * >( data_ )->size } : ByteView{};
}


ConstByteView SharedBufferPtr::get_const_view() const noexcept
{
     return data_ ? ConstByteView{ data_ + sizeof( ControlBlock ),
          reinterpret_cast< ControlBlock * >( data_ )->size } : ConstByteView{};
}

} // namespace _16nar::memory
