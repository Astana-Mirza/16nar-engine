#include <catch2/catch_test_macros.hpp>
#include <16nar/render/opengl/st_resource_manager.h>

namespace
{

struct MockLoader
{
     static bool unload_result;
     struct HandlerType {};
     struct LoadParamsType
     {
          bool value;
     };

     static bool unload( const HandlerType& handler )
     {
          return unload_result;
     }

     static bool load( const _16nar::ResourceManagerMap&, const LoadParamsType& params, HandlerType& handler )
     {
          return params.value;
     }
};
bool MockLoader::unload_result = true;


TEST_CASE( "Load and unload", "[st_resource_manager]" )
{
     _16nar::opengl::StResourceManager< MockLoader > manager{ {} };
     MockLoader::LoadParamsType params;
     params.value = true;

     _16nar::ResID id1 = manager.load( params );
     REQUIRE( id1 == 1 );     // check ID increment

     _16nar::ResID id2 = manager.load( params );
     REQUIRE( id2 == 2 );     // check ID increment

     params.value = false;
     REQUIRE_THROWS( manager.load( params ) );    // check error handling

     REQUIRE_NOTHROW( manager.unload( id2 ) );    // should not be errors

     params.value = true;
     id2 = manager.load( params );
     REQUIRE( id2 == 2 );     // check id decrement after previous unload (so it's still 2)

     MockLoader::unload_result = false;
     REQUIRE_THROWS( manager.unload( id2 ) );     // should fail

     id2 = manager.load( params );
     REQUIRE( id2 == 2 );     // check id decrement after previous unload (so it's still 2)
}

}
