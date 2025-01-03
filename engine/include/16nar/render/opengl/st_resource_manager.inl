#ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_INL
#define _16NAR_OPENGL_ST_RESOURCE_MANAGER_INL

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

template < typename T >
StResourceManager< T >::StResourceManager( const ResourceManagerMap& managers ):
     resources_{}, managers_{ managers }, next_{ 1 }
{}


template < typename T >
StResourceManager< T >::StResourceManager( StResourceManager&& other ):
     resources_{ std::move( other.resources_ ) }, managers_{ other.managers_ }, next_{ other.next_ }
{
     other.next_ = 1; // reset default value
}


template < typename T >
StResourceManager< T >& StResourceManager< T >::operator=( StResourceManager&& rhs )
{
     if ( this == &rhs )
     {
          return *this;
     }
     clear();
     std::swap( resources_, rhs.resources_ );
     std::swap( next_, rhs.next_ );
     resources_ = rhs.resources_;
     return *this;
}


template < typename T >
StResourceManager< T >::~StResourceManager()
{
     clear();
}


template < typename T >
ResID StResourceManager< T >::load( const std::any& params )
{
     const auto *params_ptr = std::any_cast< LoadParamsType >( &params );
     if ( !params_ptr )
     {
          throw ResourceException{ "wrong resource load parameters" };
     }
     HandlerType handler;
     ResID id = next_;
     if ( id == ResID{} )     // overflow
     {
          throw ExceededIdException{};
     }
     if ( !T::load( managers_, *params_ptr, handler ) )
     {
          throw ResourceException{ "cannot load resource ", id };
     }
     auto [ it, result ] = resources_.try_emplace( id, handler );
     if ( !result )
     {
          T::unload( handler );
          throw ExceededIdException{};
     }
     ++next_;
     return id;
}


template < typename T >
void StResourceManager< T >::unload( ResID id )
{
     auto it = resources_.find( id );
     if ( it != resources_.end() )
     {
          bool ok = T::unload( it->second );
          if ( id + 1 == next_ )
          {
               --next_;
          }
          resources_.erase( it );
          if ( !ok )
          {
               throw ResourceException{ "cannot unload resource ", id };
          }
     }
     else
     {
          LOG_16NAR_WARNING( "Resource with id " << id << " does not exist, won't unload" );
     }
}


template < typename T >
void StResourceManager< T >::clear()
{
     next_ = 1;
     for ( const auto& [ id, res ] : resources_ )
     {
          if ( !T::unload( res ) )
          {
               LOG_16NAR_ERROR( "Unable to unload resource with id " << id );
          }
     }
     resources_.clear();
}


template < typename T >
std::any StResourceManager< T >::get_handler( ResID id ) const
{
     const auto iter = resources_.find( id );
     if ( iter == resources_.cend() )
     {
          throw ResourceException{ "no resource with such id ", id };
     }
     return iter->second;
}

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_INL
