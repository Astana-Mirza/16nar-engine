#include <catch2/catch_test_macros.hpp>
#include <16nar/common/math/transform_matrix.h>
#include <16nar/common/math/math_functions.h>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

TEST_CASE( "Identity matrix tests", "[transform_matrix]" )
{
     _16nar::TransformMatrix matr;
     _16nar::Vec4f vec4{ 1.0f, 2.0f, 3.0f, 4.0f };

     for ( int i = 0; i < 4; i++ )
     {
          for ( int j = 0; j < 4; j++ )
          {
               if ( i == j )
               {
                    REQUIRE( matr.get( i, j ) == 1 );
               }
               else
               {
                    REQUIRE( matr.get( i, j ) == 0 );
               }
          }
     }

     REQUIRE( matr * vec4 == vec4 );
     REQUIRE( matr * _16nar::Vec4f{} == _16nar::Vec4f{} );
}


TEST_CASE( "Translate matrix tests", "[transform_matrix]" )
{
     _16nar::TransformMatrix matr;
     _16nar::Vec4f vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

     SECTION( "2D translation" )
     {
          matr.move( _16nar::Vec2f{ 64.35f, -156.32f } );
          REQUIRE( ( matr * vec4 ).equals( _16nar::Vec4f{ 65.35f, -155.32f, 1.0f, 1.0f }, TEST_PRECISION ) );
     }

     SECTION( "3D translation" )
     {
          matr.move( _16nar::Vec3f{ 41.0f, -23.5f, 100.3f } );
          REQUIRE( ( matr * vec4 ).equals( _16nar::Vec4f{ 42.0f, -22.5f, 101.3f, 1.0f }, TEST_PRECISION ) );
     }
}


TEST_CASE( "Scale matrix tests", "[transform_matrix]" )
{
     _16nar::TransformMatrix matr;
     _16nar::Vec4f vec4{ 1.0f, 2.0f, 3.0f, 1.0f };
     _16nar::Vec3f vec3{ 1.0f, 2.0f, 3.0f };
     _16nar::Vec2f vec2{ 1.0f, 2.0f };
     SECTION( "2D scale" )
     {
          matr.scale( _16nar::Vec2f{ 3.0f, 2.0f } );
          REQUIRE( ( matr * vec4 ).equals( _16nar::Vec4f{ 3.0f, 4.0f, 3.0f, 1.0f }, TEST_PRECISION ) );
          REQUIRE( ( matr * vec2 ).equals( _16nar::Vec2f{ 3.0f, 4.0f }, TEST_PRECISION ) );
     }

     SECTION( "3D scale" )
     {
          matr.scale( _16nar::Vec3f{ 3.0f, 2.0f, 1.5f } );
          REQUIRE( ( matr * vec4 ).equals( _16nar::Vec4f{ 3.0f, 4.0f, 4.5f, 1.0f }, TEST_PRECISION ) );
          REQUIRE( ( matr * vec3 ).equals( _16nar::Vec3f{ 3.0f, 4.0f, 4.5f }, TEST_PRECISION ) );
     }

     SECTION( "2D scale with pivot" )
     {
          matr.scale( _16nar::Vec2f{ 3.0f, 2.0f }, _16nar::Vec2f{ 1.0f, 1.0f } );
          REQUIRE( ( matr * vec4 ).equals( _16nar::Vec4f{ 1.0f, 3.0f, 3.0f, 1.0f }, TEST_PRECISION ) );
          REQUIRE( ( matr * vec2 ).equals( _16nar::Vec2f{ 1.0f, 3.0f }, TEST_PRECISION ) );
     }

     SECTION( "3D scale with pivot" )
     {
          matr.scale( _16nar::Vec3f{ 3.0f, 2.0f, 4.0f }, _16nar::Vec3f{ -1.0f, 1.0f, 2.0f } );
          REQUIRE( ( matr * vec4 ).equals( _16nar::Vec4f{ 5.0f, 3.0f, 6.0f, 1.0f }, TEST_PRECISION ) );
          REQUIRE( ( matr * vec3 ).equals( _16nar::Vec3f{ 5.0f, 3.0f, 6.0f }, TEST_PRECISION ) );
     }
}


TEST_CASE( "Rotation matrix tests", "[transform_matrix]" )
{
     _16nar::TransformMatrix matr;
     _16nar::Vec4f vec4{ 1.0f, 2.0f, 3.0f, 1.0f };
     _16nar::Vec3f vec3{ 1.0f, 2.0f, 3.0f };
     _16nar::Vec2f vec2{ 1.0f, 2.0f };
     float s3 = std::sqrt( 3.0f );
     float s2 = std::sqrt( 2.0f );

     SECTION( "2D Rotation" )
     {
          matr.rotate( _16nar::deg2rad( 30 ) );
          REQUIRE( ( matr * vec2 ).equals( _16nar::Vec2f{ s3 / 2 - 1, 0.5f + s3 }, TEST_PRECISION ) );
     }

     SECTION( "3D Rotation" )
     {
          matr.rotate( _16nar::deg2rad( 45 ), _16nar::Vec3f{ 0.0f, 1.0f, 0.0f } );
          REQUIRE( ( matr * vec3 ).equals( _16nar::Vec3f{ 2*s2, 2.0f, s2 }, TEST_PRECISION ) );
     }

     SECTION( "2D Rotation with pivot" )
     {
          matr.rotate( _16nar::deg2rad( 45 ), _16nar::Vec2f{ 1.0f, 1.0f } );
          REQUIRE( ( matr * vec2 ).equals( _16nar::Vec2f{ 1 - s2 / 2, 1 + s2 / 2 }, TEST_PRECISION ) );
     }

     SECTION( "3D Rotation with pivot" )
     {
          matr.rotate( _16nar::deg2rad( 45 ), _16nar::Vec3f{ 1.0f, 0.0f, 0.0f }, _16nar::Vec3f{ 1.0f, 1.0f, 2.0f } );
          REQUIRE( ( matr * vec3 ).equals( _16nar::Vec3f{ 1.0f, 1.0f, s2 + 2 }, TEST_PRECISION ) );
     }
}


TEST_CASE( "Matrix functions", "[transform_matrix]" )
{
     _16nar::TransformMatrix matr{      // affine transformations only
          1.0f, -2.0f,  3.0f,  0.0f,
          5.0f,  6.0f,  7.0f,  0.0f,
          9.0f, -1.0f, -2.0f,  0.0f,
         -4.0f,  8.0f,  4.0f,  1.0f
     };

     _16nar::TransformMatrix inv{
            5.0f / 328.0f,   7.0f / 328.0f,  4.0f / 41.0f, 0.0f,
          -73.0f / 328.0f,  29.0f / 328.0f, -1.0f / 41.0f, 0.0f,
           59.0f / 328.0f,  17.0f / 328.0f, -2.0f / 41.0f, 0.0f,
           46.0f /  41.0f, -34.0f /  41.0f, 32.0f / 41.0f, 1.0f
     };

     SECTION( "Inverse matrix" )
     {
          REQUIRE( matr.inv().equals( inv, TEST_PRECISION ) );
          REQUIRE( matr.affine_inv().equals( inv, TEST_PRECISION ) );
          REQUIRE( inv.inv().equals( matr, TEST_PRECISION ) );
          REQUIRE( inv.affine_inv().equals( matr, TEST_PRECISION ) );
     }

     SECTION( "Determinant" )
     {
          REQUIRE( matr.det() == -328.0f );
          REQUIRE( matr.det() == 1.0f / inv.det() );
     }

     SECTION( "Transpose" )
     {
          _16nar::TransformMatrix tr{
                1.0f, 5.0f,  9.0f, -4.0f,
               -2.0f, 6.0f, -1.0f,  8.0f,
                3.0f, 7.0f, -2.0f,  4.0f,
                0.0f, 0.0f,  0.0f,  1.0f
          };

          REQUIRE( matr.transpose().equals( tr, TEST_PRECISION ) );
          REQUIRE( matr.inv_transpose().equals( matr.inv().transpose(), TEST_PRECISION ) );
     }
}
