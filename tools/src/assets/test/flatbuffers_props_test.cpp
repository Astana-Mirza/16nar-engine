#include <catch2/catch_test_macros.hpp>

#include <16nar/tools/assets/flatbuffers/flatbuffers_props_reader.h>
#include <16nar/tools/assets/flatbuffers/flatbuffers_props_writer.h>

#include <cmath>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

namespace fs = std::filesystem;

namespace
{

TEST_CASE( "Properties reading and writing in flatbuffers format", "[flatbuffers_props]" )
{
     _16nar::tools::FlatBuffersPropsWriter writer{};
     writer.set_uint64( "uint64", 1234 );
     writer.set_uint32( "uint32", 5678 );
     writer.set_uint16( "uint16", 65535 );
     writer.set_uint8( "uint8", 255 );
     writer.set_int64( "int64", -1234 );
     writer.set_int32( "int32", 5432 );
     writer.set_int16( "int16", -67 );
     writer.set_int8( "int8", -4 );
     writer.set_bool( "bool", true );
     writer.set_float( "float", 12.34f );
     writer.set_double( "double", -43.21 );
     writer.set_string( "string", "rock'n'roll \"" );
     writer.set_string( "empty_string", std::string{} );

     writer.set_uint64_arr( "uint64_arr", std::vector< uint64_t >{ 256, 614, 0, 54, 122 } );
     writer.set_uint32_arr( "uint32_arr", std::vector< uint32_t >{ 12, 41, 60, 17, 52 } );
     writer.set_uint16_arr( "uint16_arr", std::vector< uint16_t >{ 444, 10, 0, 17 } );
     writer.set_uint8_arr( "uint8_arr", std::vector< uint8_t >{ 10, 0, 54, 255 } );
     writer.set_int64_arr( "int64_arr", std::vector< int64_t >{ -625, -61, 0, 564, 122 } );
     writer.set_int32_arr( "int32_arr", std::vector< int32_t >{ -2, -1, 0, 1, 2 } );
     writer.set_int16_arr( "int16_arr", std::vector< int16_t >{ -44, 10, 0, 17 } );
     writer.set_int8_arr( "int8_arr", std::vector< int8_t >{ -4, 0, 1, 127 } );
     writer.set_bool_arr( "bool_arr", std::vector< bool >{ true, false, true } );
     writer.set_float_arr( "float_arr", std::vector< float >{ 12.34f, 66.77f, -4.25f } );
     writer.set_double_arr( "double_arr", std::vector< double >{ -43.21, 55.66, 0.0 } );
     writer.set_string_arr( "string_arr", std::vector< std::string >{ "hard rock", "", "metal" } );

     writer.set_vec2f( "vec2f", _16nar::Vec2f{ 5.75f, 2.5f } );
     writer.set_vec3f( "vec3f", _16nar::Vec3f{ -0.75f, 24.45f, 56.0f } );
     writer.set_vec4f( "vec4f", _16nar::Vec4f{ 0.0f, 7.7f, 0.45f, 56.001f } );
     writer.set_vec2i( "vec2i", _16nar::Vec2i{ -556, 978 } );
     writer.set_vec3i( "vec3i", _16nar::Vec3i{ 398, 1095, 0 } );
     writer.set_vec4i( "vec4i", _16nar::Vec4i{ 1, 5, 7847, -5 } );

     writer.set_float_rect( "float_rect", _16nar::FloatRect{
          _16nar::Vec2f{ 2.5f, 0.0f }, _16nar::Vec2f{ 100.25f, 56.0f } } );
     writer.set_int_rect( "int_rect", _16nar::IntRect{
          _16nar::Vec2i{ 2, -10 }, _16nar::Vec2i{ 100, 50 } } );
     writer.set_resource_index( "resource_index", _16nar::tools::ResourceIndex{ 4, 7 } );

     auto result = writer.finish_and_get_result();

     _16nar::tools::FlatBuffersPropsReader reader{
          reinterpret_cast< const std::byte* >( result.data() ), result.size(), false };

     REQUIRE( !reader.is_owner() );
     REQUIRE( reader.get_uint64( "uint64" ).value() == 1234 );
     REQUIRE( reader.get_uint64( "no_uint64" ).has_value() == false );
     REQUIRE( reader.get_uint32( "uint32" ).value() == 5678 );
     REQUIRE( reader.get_uint32( "no_uint32" ).has_value() == false );
     REQUIRE( reader.get_uint16( "uint16" ).value() == 65535 );
     REQUIRE( reader.get_uint16( "no_uint16" ).has_value() == false );
     REQUIRE( reader.get_uint8( "uint8" ).value() == 255 );
     REQUIRE( reader.get_uint8( "no_uint8" ).has_value() == false );
     REQUIRE( reader.get_int64( "int64" ).value() == -1234 );
     REQUIRE( reader.get_int64( "no_int64" ).has_value() == false );
     REQUIRE( reader.get_int32( "int32" ).value() == 5432 );
     REQUIRE( reader.get_int32( "no_int32" ).has_value() == false );
     REQUIRE( reader.get_int16( "int16" ).value() == -67 );
     REQUIRE( reader.get_int16( "no_int16" ).has_value() == false );
     REQUIRE( reader.get_int8( "int8" ).value() == -4 );
     REQUIRE( reader.get_int8( "no_int8" ).has_value() == false );
     REQUIRE( reader.get_bool( "bool" ).value() == true );
     REQUIRE( reader.get_bool( "no_bool" ).has_value() == false );
     REQUIRE( std::fabs( reader.get_float( "float" ).value() - 12.34f ) <= TEST_PRECISION );
     REQUIRE( reader.get_float( "no_float" ).has_value() == false );
     REQUIRE( std::fabs( reader.get_double( "double" ).value() - -43.21 ) <= TEST_PRECISION );
     REQUIRE( reader.get_double( "no_double" ).has_value() == false );
     REQUIRE( reader.get_string( "string" ).value() == "rock'n'roll \"" );
     REQUIRE( reader.get_string( "no_string" ).has_value() == false );
     REQUIRE( reader.get_string( "empty_string" ).value().empty() ); // it must exist, but be empty

     REQUIRE( reader.get_uint64_arr( "uint64_arr" ).value() == std::vector< uint64_t >{ 256, 614, 0, 54, 122 } );
     REQUIRE( reader.get_uint64_arr( "no_uint64_arr" ).has_value() == false );
     REQUIRE( reader.get_uint32_arr( "uint32_arr" ).value() == std::vector< uint32_t >{ 12, 41, 60, 17, 52 } );
     REQUIRE( reader.get_uint32_arr( "no_uint32_arr" ).has_value() == false );
     REQUIRE( reader.get_uint16_arr( "uint16_arr" ).value() == std::vector< uint16_t >{ 444, 10, 0, 17 } );
     REQUIRE( reader.get_uint16_arr( "no_uint16_arr" ).has_value() == false );
     REQUIRE( reader.get_uint8_arr( "uint8_arr" ).value() == std::vector< uint8_t >{ 10, 0, 54, 255 } );
     REQUIRE( reader.get_uint8_arr( "no_uint8_arr" ).has_value() == false );
     REQUIRE( reader.get_int64_arr( "int64_arr" ).value() == std::vector< int64_t >{ -625, -61, 0, 564, 122 } );
     REQUIRE( reader.get_int64_arr( "no_int64_arr" ).has_value() == false );
     REQUIRE( reader.get_int32_arr( "int32_arr" ).value() == std::vector< int32_t >{ -2, -1, 0, 1, 2 } );
     REQUIRE( reader.get_int32_arr( "no_int32_arr" ).has_value() == false );
     REQUIRE( reader.get_int16_arr( "int16_arr" ).value() == std::vector< int16_t >{ -44, 10, 0, 17 } );
     REQUIRE( reader.get_int16_arr( "no_int16_arr" ).has_value() == false );
     REQUIRE( reader.get_int8_arr( "int8_arr" ).value() == std::vector< int8_t >{ -4, 0, 1, 127 } );
     REQUIRE( reader.get_int8_arr( "no_int8_arr" ).has_value() == false );
     REQUIRE( reader.get_bool_arr( "bool_arr" ).value() == std::vector< bool >{ true, false, true } );
     REQUIRE( reader.get_bool_arr( "no_bool_arr" ).has_value() == false );

     auto float_arr = reader.get_float_arr( "float_arr" ).value();
     REQUIRE( float_arr.size() == 3 );
     REQUIRE( std::fabs( float_arr.at( 0 ) - 12.34f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( float_arr.at( 1 ) - 66.77f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( float_arr.at( 2 ) - -4.25f ) <= TEST_PRECISION );
     REQUIRE( reader.get_float( "no_float_arr" ).has_value() == false );

     auto double_arr = reader.get_float_arr( "double_arr" ).value();
     REQUIRE( double_arr.size() == 3 );
     REQUIRE( std::fabs( double_arr.at( 0 ) - -43.21 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( double_arr.at( 1 ) - 55.66 ) <= TEST_PRECISION );
     REQUIRE( std::fabs( double_arr.at( 2 ) - 0.0 ) <= TEST_PRECISION );
     REQUIRE( reader.get_double_arr( "no_double_arr" ).has_value() == false );

     REQUIRE( reader.get_string_arr( "string_arr" ).value() == std::vector< std::string >{ "hard rock", "", "metal" } );
     REQUIRE( reader.get_string_arr( "no_string_arr" ).has_value() == false );

     auto vec2f = reader.get_vec2f( "vec2f" ).value();
     REQUIRE( reader.get_vec2f( "no_vec2f" ).has_value() == false );
     auto vec3f = reader.get_vec3f( "vec3f" ).value();
     REQUIRE( reader.get_vec3f( "no_vec3f" ).has_value() == false );
     auto vec4f = reader.get_vec4f( "vec4f" ).value();
     REQUIRE( reader.get_vec4f( "no_vec4f" ).has_value() == false );

     REQUIRE( std::fabs( vec2f.x() - 5.75f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec2f.y() - 2.5f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec3f.x() - -0.75f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec3f.y() - 24.45f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec3f.z() - 56.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec4f.x() - 0.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec4f.y() - 7.7f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec4f.z() - 0.45f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( vec4f.w() - 56.001f ) <= TEST_PRECISION );

     REQUIRE( reader.get_vec2i( "vec2i" ).value() == _16nar::Vec2i{ -556, 978 } );
     REQUIRE( reader.get_vec2i( "no_vec2i" ).has_value() == false );
     REQUIRE( reader.get_vec3i( "vec3i" ).value() == _16nar::Vec3i{ 398, 1095, 0 } );
     REQUIRE( reader.get_vec3i( "no_vec3i" ).has_value() == false );
     REQUIRE( reader.get_vec4i( "vec4i" ).value() == _16nar::Vec4i{ 1, 5, 7847, -5 } );
     REQUIRE( reader.get_vec4i( "no_vec4i" ).has_value() == false );

     auto float_rect = reader.get_float_rect( "float_rect" ).value();
     REQUIRE( reader.get_float_rect( "no_float_rect" ).has_value() == false );
     REQUIRE( std::fabs( float_rect.get_pos().x() - 2.5f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( float_rect.get_pos().y() - 0.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( float_rect.get_end().x() - 100.25f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( float_rect.get_end().y() - 56.0f ) <= TEST_PRECISION );
     REQUIRE( reader.get_int_rect( "int_rect" ).value() ==  _16nar::IntRect{
          _16nar::Vec2i{ 2, -10 }, _16nar::Vec2i{ 100, 50 } } );
     REQUIRE( reader.get_int_rect( "no_int_rect" ).has_value() == false );
     auto resource_index = reader.get_resource_index( "resource_index" ).value();
     REQUIRE( resource_index.package == 4 );
     REQUIRE( resource_index.resource == 7 );
     REQUIRE( reader.get_resource_index( "no_resource_index" ).has_value() == false );
}

}
