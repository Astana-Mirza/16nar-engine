#include <catch2/catch_test_macros.hpp>

#include <16nar/constructor2d/system/scene.h>
#include <16nar/constructor2d/system/scene_state.h>

TEST_CASE( "Scene states", "[constructor2d_scene]" )
{
     _16nar::constructor2d::Scene scene{};
     scene.register_state( _16nar::constructor2d::SceneState{
          15, std::unique_ptr< _16nar::constructor2d::IRenderSystem2D >{}, false, true
     } );
     scene.register_state( _16nar::constructor2d::SceneState{
          5, std::unique_ptr< _16nar::constructor2d::IRenderSystem2D >{}, false, false
     } );

     REQUIRE( scene.get_state( 15 ).get_order() == 15 );
     REQUIRE( scene.get_state( 15 ).get_rendering() );
     REQUIRE( !scene.get_state( 15 ).get_updating() );

     REQUIRE( scene.get_state( 5 ).get_order() == 5 );
     REQUIRE( !scene.get_state( 5 ).get_rendering() );
     REQUIRE( !scene.get_state( 5 ).get_updating() );
}
