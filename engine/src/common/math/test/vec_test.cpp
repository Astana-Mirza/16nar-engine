#include <catch2/catch_test_macros.hpp>
#include <16nar/common/math/vec.h>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

TEST_CASE( "Vector construction test", "[vec]" )
{
     SECTION( "Default construction" )
     {
          _16nar::Vec2f vec2f;
          _16nar::Vec3f vec3f;
          _16nar::Vec4f vec4f;
          _16nar::Vec2i vec2i;
          _16nar::Vec3i vec3i;
          _16nar::Vec4i vec4i;

          REQUIRE( vec2f[ 0 ] == 0.0f );
          REQUIRE( vec2f[ 1 ] == 0.0f );
          REQUIRE( vec3f[ 0 ] == 0.0f );
          REQUIRE( vec3f[ 1 ] == 0.0f );
          REQUIRE( vec3f[ 2 ] == 0.0f );
          REQUIRE( vec4f[ 0 ] == 0.0f );
          REQUIRE( vec4f[ 1 ] == 0.0f );
          REQUIRE( vec4f[ 2 ] == 0.0f );
          REQUIRE( vec4f[ 3 ] == 0.0f );

          REQUIRE( vec2i[ 0 ] == 0 );
          REQUIRE( vec2i[ 1 ] == 0 );
          REQUIRE( vec3i[ 0 ] == 0 );
          REQUIRE( vec3i[ 1 ] == 0 );
          REQUIRE( vec3i[ 2 ] == 0 );
          REQUIRE( vec4i[ 0 ] == 0 );
          REQUIRE( vec4i[ 1 ] == 0 );
          REQUIRE( vec4i[ 2 ] == 0 );
          REQUIRE( vec4i[ 3 ] == 0 );
     }

     SECTION( "Regular construction" )
     {
          _16nar::Vec2f vec2f{ -1.3f, 2.5f };
          _16nar::Vec3f vec3f( 1.0f, 2.0f, 23.4f );
          _16nar::Vec4f vec4f{ 9.3f, 0.0f, 12.3f, -7.3f };
          _16nar::Vec2i vec2i( 6, 7 );
          _16nar::Vec3i vec3i{ -3, 4, 2 };
          _16nar::Vec4i vec4i( 11, 543, 85, 54 );

          REQUIRE( vec2f[ 0 ] == -1.3f );
          REQUIRE( vec2f[ 1 ] == 2.5f );
          REQUIRE( vec3f[ 0 ] == 1.0f );
          REQUIRE( vec3f[ 1 ] == 2.0f );
          REQUIRE( vec3f[ 2 ] == 23.4f );
          REQUIRE( vec4f[ 0 ] == 9.3f );
          REQUIRE( vec4f[ 1 ] == 0.0f );
          REQUIRE( vec4f[ 2 ] == 12.3f );
          REQUIRE( vec4f[ 3 ] == -7.3f );

          REQUIRE( vec2i[ 0 ] == 6 );
          REQUIRE( vec2i[ 1 ] == 7 );
          REQUIRE( vec3i[ 0 ] == -3 );
          REQUIRE( vec3i[ 1 ] == 4 );
          REQUIRE( vec3i[ 2 ] == 2 );
          REQUIRE( vec4i[ 0 ] == 11 );
          REQUIRE( vec4i[ 1 ] == 543 );
          REQUIRE( vec4i[ 2 ] == 85 );
          REQUIRE( vec4i[ 3 ] == 54 );
     }

     SECTION( "Copy construction" )
     {
          _16nar::Vec3f copy3f{ -1, 0, 10 };
          _16nar::Vec3f vec3f = copy3f;
          REQUIRE( vec3f[ 0 ] == -1.0f );
          REQUIRE( vec3f[ 1 ] == 0.0f );
          REQUIRE( vec3f[ 2 ] == 10.0f );

          _16nar::Vec2i copy2i{ 6, 7 };
          _16nar::Vec2i vec2i{ copy2i };
          REQUIRE( vec2i[ 0 ] == 6 );
          REQUIRE( vec2i[ 1 ] == 7 );
     }
}


TEST_CASE( "Vector operators test", "[vec]" )
{
     _16nar::Vec2f lhsf{ 1.0f, 2.0f };
     _16nar::Vec2f rhsf{ 2.0f, 1.0f };

     _16nar::Vec3i lhs{ -1, 0, 3 };
     _16nar::Vec3i rhs{ 4, -3, 1 };



     SECTION( "Equality" )
     {
          REQUIRE( lhsf != rhsf );
          REQUIRE( lhsf == lhsf );
          REQUIRE( rhsf == _16nar::Vec2f{ 2.0f, 1.0f } );

          REQUIRE( lhs != rhs );
          REQUIRE( lhs == lhs );
          REQUIRE( rhs == _16nar::Vec3i{ 4, -3, 1 } );
     }

     SECTION( "Multiplication by scalar" )
     {
          REQUIRE( rhsf * 2.0f == _16nar::Vec2f{ 4.0f, 2.0f } );
          REQUIRE( 2.0f * rhsf == _16nar::Vec2f{ 4.0f, 2.0f } );

          REQUIRE( lhs * 7 == _16nar::Vec3i{ -7, 0, 21 } );
          REQUIRE( 5 * rhs == _16nar::Vec3i{ 20, -15, 5 } );
     }

     SECTION( "Addition and subtraction" )
     {
          REQUIRE( ( lhsf + rhsf ).equals( _16nar::Vec2f{ 3.0f, 3.0f }, TEST_PRECISION ) );
          REQUIRE( ( lhsf - rhsf ).equals( _16nar::Vec2f{ -1.0f, 1.0f }, TEST_PRECISION ) );
          lhsf -= _16nar::Vec2f{ 5, 9 };
          REQUIRE( lhsf.equals( _16nar::Vec2f{ -4.0f, -7.0f }, TEST_PRECISION ) );
          rhsf += _16nar::Vec2f{ 3, 4 };
          REQUIRE( rhsf.equals( _16nar::Vec2f{ 5.0f, 5.0f }, TEST_PRECISION ) );
     }

     SECTION( "Negation" )
     {
          _16nar::Vec2f nlhsf = -lhsf;
          _16nar::Vec3i nlhs = -lhs;
          REQUIRE( nlhsf[ 0 ] == -lhsf[ 0 ] );
          REQUIRE( nlhsf[ 1 ] == -lhsf[ 1 ] );
          REQUIRE( nlhs[ 0 ] == -lhs[ 0 ] );
          REQUIRE( nlhs[ 1 ] == -lhs[ 1 ] );
          REQUIRE( nlhs[ 2 ] == -lhs[ 2 ] );
     }
}


TEST_CASE( "Vector functions test", "[vec]" )
{
     _16nar::Vec3f vec3f{ 1, 1, 1 };
     _16nar::Vec2f vec2f{ 3, 2 };
     _16nar::Vec3f d3f{ 3, 4, 5 };
     _16nar::Vec2f d2f{ -1, 4 };

     SECTION( "Length" )
     {
          REQUIRE( vec3f.length() == std::sqrt( 3.0f ) );
          REQUIRE( vec2f.length() == std::sqrt( 13.0f ) );
     }

     SECTION( "Normalize" )
     {
          float len3 = vec3f.length();
          float len2 = vec2f.length();
          _16nar::Vec3f vec3norm = vec3f.normalize();
          _16nar::Vec2f vec2norm = vec2f.normalize();

          REQUIRE( vec3norm[ 0 ] == vec3f[ 0 ] / len3 );
          REQUIRE( vec3norm[ 1 ] == vec3f[ 1 ] / len3 );
          REQUIRE( vec3norm[ 2 ] == vec3f[ 2 ] / len3 );

          REQUIRE( vec2norm[ 0 ] == vec2f[ 0 ] / len2 );
          REQUIRE( vec2norm[ 1 ] == vec2f[ 1 ] / len2 );
     }

     SECTION( "Distance" )
     {
          REQUIRE( vec3f.distance( d3f ) == ( vec3f - d3f ).length() );
          REQUIRE( vec2f.distance( d2f ) == ( vec2f - d2f ).length() );
     }

     SECTION( "Dot" )
     {
          REQUIRE( std::fabs( vec3f.dot( d3f ) - 12.0f ) < TEST_PRECISION );
          REQUIRE( std::fabs( vec2f.dot( d2f ) - 5.0f ) < TEST_PRECISION );
     }

     SECTION( "Cross" )
     {
          REQUIRE( vec3f.cross( d3f ).equals( _16nar::Vec3f{ 1, -2, 1 }, TEST_PRECISION ) );
     }

     SECTION( "Faceforward" )
     {
          REQUIRE( vec3f.faceforward( _16nar::Vec3f{ 0, 1, 1 }, _16nar::Vec3f{ 1,  0, 1 } ) == -vec3f ); // dot == 1
          REQUIRE( vec3f.faceforward( _16nar::Vec3f{ 0, 1, 1 }, _16nar::Vec3f{ 1, -1, 0 } ) ==  vec3f ); // dot == -1
          REQUIRE( vec3f.faceforward( _16nar::Vec3f{ 0, 1, 1 }, _16nar::Vec3f{ 1, -1, 1 } ) == -vec3f ); // dot == 0

          REQUIRE( vec2f.faceforward( _16nar::Vec2f{ -1, -1 }, _16nar::Vec2f{ 0,  1 } ) ==  vec2f ); // dot == -1
          REQUIRE( vec2f.faceforward( _16nar::Vec2f{ -1, -1 }, _16nar::Vec2f{ 2, -2 } ) == -vec2f ); // dot == 0
          REQUIRE( vec2f.faceforward( _16nar::Vec2f{  1,  0 }, _16nar::Vec2f{ 1,  1 } ) == -vec2f ); // dot == 1
     }

     SECTION( "Reflect" )
     {
          _16nar::Vec3f vec3norm = d3f.normalize();
          _16nar::Vec2f vec2norm = d2f.normalize();
          REQUIRE( vec3f.reflect( vec3norm ).equals( vec3f - 2 * vec3norm.dot( vec3f ) * vec3norm, TEST_PRECISION ) );
          REQUIRE( vec2f.reflect( vec2norm ).equals( vec2f - 2 * vec2norm.dot( vec2f ) * vec2norm, TEST_PRECISION ) );
     }

     SECTION( "Refract" )
     {
          auto calc_refract = []( const auto& vec, const auto& norm, float eta )
          {
               float dot = vec.dot( norm );
               return -std::sqrt( 1 - eta * eta * ( 1 - dot * dot ) ) * norm + eta * ( vec - dot * norm );
          };
          const float eta = 0.1f;
          _16nar::Vec3f vec3norm = d3f.normalize();
          _16nar::Vec2f vec2norm = d2f.normalize();
          REQUIRE( vec3f.refract( vec3norm, eta ).equals( calc_refract( vec3f, vec3norm, eta ), TEST_PRECISION ) );
          REQUIRE( vec2f.refract( vec2norm, eta ).equals( calc_refract( vec2f, vec2norm, eta ), TEST_PRECISION ) );
     }
}
