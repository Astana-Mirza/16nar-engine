#include <16nar/core/resources/resource_handle.h>

#include <16nar/core/resources/iresource_manager.h>
#include <16nar/platform/logger/logger.h>

namespace _16nar::resources
{

ResourceHandle::ResourceHandle() noexcept:
     manager_{}, id_{}
{}


ResourceHandle::~ResourceHandle()
{
     if ( *this )
     {
          manager_->dec_ref( id_ );
     }
}


ResourceHandle::ResourceHandle( const ResourceHandle& other ):
     manager_{ other.manager_ }, id_{ other.id_ }
{
     if ( *this )
     {
          manager_->inc_ref( id_ );
     }
}


ResourceHandle::ResourceHandle( ResourceHandle&& other ):
     manager_{ other.manager_ }, id_{ other.id_ }
{
     other.manager_ = nullptr;
     other.id_ = 0;
}


ResourceHandle& ResourceHandle::operator=( const ResourceHandle& rhs )
{
     if ( this != &rhs )
     {
          if ( *this )
          {
               manager_->dec_ref( id_ );
          }
          manager_ = rhs.manager_;
          id_ = rhs.id_;
          if ( *this )
          {
               manager_->inc_ref( id_ );
          }
     }
     return *this;
}


ResourceHandle& ResourceHandle::operator=( ResourceHandle&& rhs )
{
     if ( this != &rhs )
     {
          if ( *this )
          {
               manager_->dec_ref( id_ );
          }
          manager_ = rhs.manager_;
          id_ = rhs.id_;
          rhs.manager_ = nullptr;
          rhs.id_ = 0;
     }
     return *this;
}


bool ResourceHandle::load( execution::Mode mode )
{
     if ( !*this )
     {
          LOG_16NAR_ERROR( "Cannot load on empty resource handle" );
          return false;
     }
     return manager_->load_by_id( id_, mode );
}


bool ResourceHandle::wait_preload()
{
     if ( !*this )
     {
          LOG_16NAR_ERROR( "Cannot wait_preload on empty resource handle" );
          return false;
     }
     return manager_->wait_preload( id_ );
}


bool ResourceHandle::wait_load()
{
     if ( !*this )
     {
          LOG_16NAR_ERROR( "Cannot wait_load on empty resource handle" );
          return false;
     }
     return manager_->wait_load( id_ );
}


ResourceHandle::ResourceHandle( IResourceManager *manager, ResID id ) noexcept:
     manager_{ manager }, id_{ id }
{}

} // namespace _16nar::resources
