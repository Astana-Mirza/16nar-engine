#include <catch2/catch_test_macros.hpp>
#include <16nar/math/math_functions.h>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

TEST_CASE( "Radians and degrees", "[math_functions]" )
{
     constexpr float pi = 3.14159265358979f;

     REQUIRE( _16nar::deg2rad( 0 ) == 0 );
     REQUIRE( std::fabs( _16nar::deg2rad( 30 ) - pi / 6 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::deg2rad( 45 ) - pi / 4 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::deg2rad( 60 ) - pi / 3 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::deg2rad( 90 ) - pi / 2 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::deg2rad( 120 ) - 2 * pi / 3 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::deg2rad( 180 ) - pi ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::deg2rad( 360 ) - 2 * pi ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::deg2rad( 720 ) - 4 * pi ) <= TEST_PRECISION );

     REQUIRE( _16nar::rad2deg( 0 ) == 0 );
     REQUIRE( std::fabs( _16nar::rad2deg( pi / 6 ) - 30 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::rad2deg( pi / 4 ) - 45 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::rad2deg( pi / 3 ) - 60 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::rad2deg( pi / 2 ) - 90 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::rad2deg( 2 * pi / 3 ) - 120 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::rad2deg( pi ) - 180 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::rad2deg( 2 * pi ) - 360 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( _16nar::rad2deg( 4 * pi ) - 720 ) <= TEST_PRECISION );
}
