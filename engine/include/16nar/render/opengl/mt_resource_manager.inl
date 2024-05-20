#ifndef _16NAR_OPENGL_MT_RESOURCE_MANAGER_INL
#define _16NAR_OPENGL_MT_RESOURCE_MANAGER_INL

#include <16nar/system/exceptions.h>

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
     load_queue_[ next_index ].push( std::make_pair( id, std::any_cast< const T& >( params ) ) );
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
               // todo: log
          }
     }
     resources_.clear();
}


template < typename T >
void MtResourceManager< T >::process_load_queue()
{
     auto& queue = load_queue_[ frame_index_ ];
     while ( !queue.empty() )
     {
          auto& request = queue.front();
          if ( !request.second.load() )
          {
               throw ResourceException{ "cannot load resource ", request.first };
          }
          auto [ it, result ] = resources_.try_emplace( request.first, request.second.get_handler() );
          if ( !result )
          {
               T::unload( request.second.get_handler() );
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
