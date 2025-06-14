#include <catch2/catch_test_macros.hpp>

#include <16nar/tools/constructor2d/json/json_node_package_reader.h>

#include <fstream>
#include <nlohmann/json.hpp>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

namespace
{

TEST_CASE( "Node package reading in json format", "[json_node_package]" )
{
     std::ifstream file( "data/test_node_package.json" );
     auto json = nlohmann::json::parse( file );
     _16nar::tools::constructor2d::JsonNodePackageReader scene_reader{ std::move( json ) };

     auto deps = scene_reader.get_dependencies();
     REQUIRE( deps.resources.size() == 1 );
     REQUIRE( deps.resources[ 0 ].name == "test_package" );
     REQUIRE( deps.resources[ 0 ].elements.size() == 2 );
     REQUIRE( deps.resources[ 0 ].elements[ 0 ] == "test_texture" );
     REQUIRE( deps.resources[ 0 ].elements[ 1 ] == "test_shader" );
     REQUIRE( deps.libs.size() == 0 );
     REQUIRE( deps.pieces.size() == 1 );
     REQUIRE( deps.pieces[ 0 ].name == "test_node_package2" );
     REQUIRE( deps.pieces[ 0 ].elements.size() == 1 );
     REQUIRE( deps.pieces[ 0 ].elements[ 0 ] == "test_node_2d" );

     REQUIRE( scene_reader.get_scene_piece_count() == 1 );

     auto& piece_reader = scene_reader.get_scene_piece_reader( "test_piece" );
     REQUIRE( piece_reader.is_empty() == false );

     auto& node_reader1 = piece_reader.get_current_node_reader();
     REQUIRE( piece_reader.get_current_node_type() == _16nar::tools::constructor2d::NodeType::Node2D );
     const auto pos = node_reader1.get_vec2f( "pos" ).value();
     REQUIRE( std::fabs( pos.x() - 10.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( pos.y() - -50.0f ) <= TEST_PRECISION );
     const auto scale1 = node_reader1.get_vec2f( "scale" ).value();
     REQUIRE( std::fabs( scale1.x() - 3.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( scale1.y() - 3.0f ) <= TEST_PRECISION );
     const auto link = node_reader1.get_resource_index( "link" ).value();
     REQUIRE( link.package == 0 );
     REQUIRE( link.resource == 0 );

     REQUIRE( piece_reader.next_node() == true );

     auto& node_reader2 = piece_reader.get_current_node_reader();
     REQUIRE( piece_reader.get_current_node_type() == _16nar::tools::constructor2d::NodeType::SpriteNode );
     const auto parent = node_reader2.get_uint32( "parent" ).value();
     REQUIRE( parent == 0 );
     const auto size = node_reader2.get_vec2f( "size" ).value();
     REQUIRE( std::fabs( size.x() - 10.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( size.y() - 10.0f ) <= TEST_PRECISION );
     const auto texture = node_reader2.get_resource_index( "texture" ).value();
     REQUIRE( texture.package == 0 );
     REQUIRE( texture.resource == 0 );
     const auto shader = node_reader2.get_resource_index( "shader" ).value();
     REQUIRE( shader.package == 0 );
     REQUIRE( shader.resource == 1 );
     const auto name = node_reader2.get_string( "name" ).value();
     REQUIRE( name == "test_sprite" );

     REQUIRE( piece_reader.next_node() == false );
}

} // anonymous namespace
