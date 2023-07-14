#include <catch2/catch_test_macros.hpp>
#include <16nar/common/math/rectangle.h>
#include <16nar/common/math/transform_matrix.h>
#include <16nar/common/math/math_functions.h>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

TEST_CASE( "Rectangle basic functions", "[rectangle]" )
{
     _16nar::FloatRect rect{ _16nar::Vec2f{ 1.0f, 1.0f }, 5.0f, 6.0f };

     REQUIRE( rect.get_pos() == _16nar::Vec2f{ 1.0f, 1.0f } );
     REQUIRE( rect.get_width() == 5.0f );
     REQUIRE( rect.get_height() == 6.0f );
}


TEST_CASE( "Rectangle area tests", "[rectangle]" )
{
     _16nar::FloatRect rect{ _16nar::Vec2f{ 1.0f, 1.0f }, 5.0f, 6.0f };

     REQUIRE( ! rect.contains( _16nar::Vec2f{ 0.0f, 0.0f } ) );
     REQUIRE( rect.contains( _16nar::Vec2f{ 1.0f, 1.0f } ) );
     REQUIRE( rect.contains( _16nar::Vec2f{ 2.0f, 4.0f } ) );
     REQUIRE( rect.contains( _16nar::Vec2f{ 6.0f, 7.0f } ) );
     REQUIRE( ! rect.contains( _16nar::Vec2f{ 6.5f, 4.0f } ) );

     REQUIRE( rect.intersects( _16nar::FloatRect{ _16nar::Vec2f{ -1.0f, -1.0f }, 3.0f, 5.0f } ) );
     REQUIRE( ! rect.intersects( _16nar::FloatRect{ _16nar::Vec2f{ 7.0f, 8.0f }, 3.0f, 5.0f } ) );
}


TEST_CASE( "Rectangle transformations", "[rectangle]" )
{
     _16nar::FloatRect rect{ _16nar::Vec2f{ 1.0f, 1.0f }, 5.0f, 6.0f };
     float s3 = std::sqrt( 3.0f );
     _16nar::FloatRect etalon{ _16nar::Vec2f{ 1.5f * s3 - 7, 1.5f + s3 },
                               7.5f * s3 + 6, 7.5f + 6 * s3 };

     _16nar::TransformMatrix matr;
     matr.scale( _16nar::Vec2f{ 3.0f, 2.0f } );
     matr.rotate( _16nar::deg2rad( 30 ) );

     auto res = matr * rect;
     REQUIRE( res.get_pos() == etalon.get_pos() );
     REQUIRE( res.get_height() == etalon.get_height() );
     REQUIRE( res.get_width() == etalon.get_width() );
}
