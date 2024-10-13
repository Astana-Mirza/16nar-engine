#include <catch2/catch_test_macros.hpp>
#include <16nar/render/opengl/mt_resource_manager.h>

#include <thread>
#include <vector>

namespace
{

struct MockLoader
{
     static bool unload_result;
     static std::size_t loaded_count;

     struct HandlerType {};
     struct LoadParamsType
     {
          bool value;
     };

     static bool unload( const HandlerType& handler )
     {
          loaded_count--;
          return unload_result;
     }

     static bool load( const _16nar::ResourceManagerMap&, const LoadParamsType& params, HandlerType& handler )
     {
          loaded_count++;
          return params.value;
     }
};
bool MockLoader::unload_result = true;
std::size_t MockLoader::loaded_count = 0;


TEST_CASE( "Parallel load and unload", "[mt_resource_manager]" )
{
     _16nar::opengl::MtResourceManager< MockLoader > manager{ {} };
     MockLoader::LoadParamsType params;
     params.value = true;

     std::vector< _16nar::ResID > ids;
     auto loader = [ &ids, &manager, &params ]( std::size_t count )
     {
          for ( std::size_t i = 0; i < count; i++ )
          {
               ids.push_back( manager.load( params ) );
          }
     };
     loader( 6 );
     REQUIRE( ids.size() == 6 );
     REQUIRE( MockLoader::loaded_count == 0 );    // requests are made but resources are not loaded

     auto queues_processor = [ &manager, &params ]()
     {
          manager.process_load_queue();
          manager.process_unload_queue();
     };

     manager.end_frame();
     std::thread t1{ loader, 5 };
     std::thread t2{ queues_processor };
     t1.join();
     t2.join();
     REQUIRE( ids.size() == 11 );
     REQUIRE( MockLoader::loaded_count == 6 );    // made all requests, loaded by previous ones

     manager.end_frame();     // synchronization point

     auto unloader = [ &ids, &manager ]()
     {
          for ( auto id : ids )
          {
               manager.unload( id );
          }
          ids.clear();
     };

     std::thread t3{ unloader };
     std::thread t4{ queues_processor };
     t3.join();
     t4.join();

     REQUIRE( MockLoader::loaded_count == 11 );   // made unload requests and loaded by previous requests.

     manager.end_frame();
     REQUIRE_NOTHROW( queues_processor() );
     REQUIRE( MockLoader::loaded_count == 0 );    // unloaded be previous requests

     params.value = false;
     REQUIRE_NOTHROW( loader( 3 ) );              // should cause error during load queue processing.
     manager.end_frame();

     params.value = true;
     ids.clear();
     loader( 3 );
     unloader();
     REQUIRE_THROWS( manager.process_load_queue() );   // should throw because of load error on previous requests.
     REQUIRE_NOTHROW( manager.process_unload_queue() );

     manager.end_frame();
     MockLoader::unload_result = false;
     REQUIRE_NOTHROW( manager.process_load_queue() );
     REQUIRE_THROWS( manager.process_unload_queue() ); // should throw because unable to unload resources.
}

}
