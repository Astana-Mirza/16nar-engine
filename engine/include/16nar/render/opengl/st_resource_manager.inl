#ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_INL
#define _16NAR_OPENGL_ST_RESOURCE_MANAGER_INL

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

template < typename T >
StResourceManager< T >::~StResourceManager()
{
     clear();
}


template < typename T >
ResID StResourceManager< T >::load( const std::any& params )
{
     const T& loader = std::any_cast< const T& >( params );
     ResID id = next_;
     if ( !loader.load() )
     {
          throw ResourceException{ "cannot load resource ", id };
     }
     auto [ it, result ] = resources_.try_emplace( id, loader.get_handler() );
     if ( !result )
     {
          T::unload( loader.get_handler() );
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

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_INL
