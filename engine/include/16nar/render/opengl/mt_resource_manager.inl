#ifndef _16NAR_OPENGL_MT_RESOURCE_MANAGER_INL
#define _16NAR_OPENGL_MT_RESOURCE_MANAGER_INL

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

template < typename T >
MtResourceManager< T >::~MtResourceManager()
{
     clear();
}


template < typename T >
ResID MtResourceManager< T >::load( const std::any& params )
{
     ResID id = next_++;
     size_t next_index = ( frame_index_ + 1 ) % _16nar_saved_frames;
     load_queue_[ next_index ].push( std::make_pair( id, std::any_cast< const LoadParamsType& >( params ) ) );
     return id;
}


template < typename T >
void MtResourceManager< T >::unload( ResID id )
{
     size_t next_index = ( frame_index_ + 1 ) % _16nar_saved_frames;
     unload_queue_[ next_index ].push( id );
}


template < typename T >
void MtResourceManager< T >::clear()
{
     next_ = 1;
     for ( auto& queue : unload_queue_ )
     {
          std::queue< ResID >{}.swap( queue );
     }
     for ( auto& queue : load_queue_ )  // do not load requested resources
     {
          std::queue< Request >{}.swap( queue );
     }
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
std::any MtResourceManager< T >::get_handler( ResID id ) const
{
     const auto iter = resources_.find( id );
     if ( iter == resources_.cend() )
     {
          throw ResourceException{ "no resource with such id ", id };
     }
     return iter->second;
}


template < typename T >
void MtResourceManager< T >::process_load_queue()
{
     auto& queue = load_queue_[ frame_index_ ];
     while ( !queue.empty() )
     {
          auto& request = queue.front();
          HandlerType handler;
          if ( !T::load( managers_, request.second, handler ) )
          {
               throw ResourceException{ "cannot load resource ", request.first };
          }
          auto [ it, result ] = resources_.try_emplace( request.first, handler );
          if ( !result )
          {
               T::unload( handler );
               queue.pop();
               throw ExceededIdException{};
          }
          queue.pop();
     }
}


template < typename T >
void MtResourceManager< T >::process_unload_queue()
{
     auto& queue = unload_queue_[ frame_index_ ];
     while ( !queue.empty() )
     {
          ResID id = queue.front();
          queue.pop();
          auto iter = resources_.find( id );
          if ( iter == resources_.end() )
          {
               LOG_16NAR_WARNING( "Resource with id " << id << " does not exist, won't unload" );
               continue;
          }
          bool ok = T::unload( iter->second );
          resources_.erase( iter );
          if ( !ok )
          {
               throw ResourceException{ "cannot unload resource ", id };
          }
     }
}


template < typename T >
void MtResourceManager< T >::end_frame()
{
     std::queue< Request >{}.swap( load_queue_[ frame_index_ ] );
     std::queue< ResID >{}.swap( unload_queue_[ frame_index_ ] );
     frame_index_ = ( frame_index_ + 1 ) % _16nar_saved_frames;
}

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_MT_RESOURCE_MANAGER_INL
