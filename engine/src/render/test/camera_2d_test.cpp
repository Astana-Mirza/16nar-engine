#include <16nar/render/camera_2d.h>
#include <catch2/catch_test_macros.hpp>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

TEST_CASE( "2D Camera", "[camera_2d]" )
{
     using namespace _16nar;

     Camera2D cam{ Vec2f{ 0.0f, 0.0f }, 200.0f, 100.0f };
     auto rect = cam.get_global_bounds();

     REQUIRE( cam.get_center().equals( Vec2f{ 0.0f, 0.0f }, TEST_PRECISION ) );
     REQUIRE( cam.get_size().equals( Vec2f{ 200.0f, 100.0f }, TEST_PRECISION ) );
     REQUIRE( rect.get_pos().equals( Vec2f{ -100.0f, -50.0f }, TEST_PRECISION ) );
     REQUIRE( std::fabs( rect.get_width() - 200.0f ) < TEST_PRECISION );
     REQUIRE( std::fabs( rect.get_height() - 100.0f ) < TEST_PRECISION );

     cam.move( Vec2f{ 50.0f, 100.0f } );
     rect = cam.get_global_bounds();

     REQUIRE( cam.get_center().equals( Vec2f{ 50.0f, 100.0f }, TEST_PRECISION ) );
     REQUIRE( rect.get_pos().equals( Vec2f{ -50.0f, 50.0f }, TEST_PRECISION ) );
     REQUIRE( std::fabs( rect.get_width() - 200.0f ) < TEST_PRECISION );
     REQUIRE( std::fabs( rect.get_height() - 100.0f ) < TEST_PRECISION );

     cam.zoom( 0.8f );
     rect = cam.get_global_bounds();

     REQUIRE( cam.get_center().equals( Vec2f{ 50.0f, 100.0f }, TEST_PRECISION ) );
     REQUIRE( rect.get_pos().equals( Vec2f{ -75.0f, 37.5f }, TEST_PRECISION ) );
     REQUIRE( std::fabs( rect.get_width() - 250.0f ) < TEST_PRECISION );
     REQUIRE( std::fabs( rect.get_height() - 125.0f ) < TEST_PRECISION );

     cam.rotate( 90.0f );
     rect = cam.get_global_bounds();

     REQUIRE( cam.get_center().equals( Vec2f{ 50.0f, 100.0f }, TEST_PRECISION ) );
     REQUIRE( rect.get_pos().equals( Vec2f{ -12.5f, -25.0f }, TEST_PRECISION ) );
     REQUIRE( std::fabs( rect.get_width() - 125.0f ) < TEST_PRECISION );
     REQUIRE( std::fabs( rect.get_height() - 250.0f ) < TEST_PRECISION );
}
