#include <catch2/catch_test_macros.hpp>

#include <16nar/tools/constructor2d/json/json_scene_reader.h>

#include <fstream>
#include <nlohmann/json.hpp>

#ifndef TEST_PRECISION
#    define TEST_PRECISION 0.00001f
#endif

namespace
{

TEST_CASE( "Scene reading in json format", "[json_scene]" )
{
     std::ifstream file( "data/test_scene.json" );
     auto json = nlohmann::json::parse( file );
     _16nar::tools::constructor2d::JsonSceneReader scene_reader{ std::move( json ) };

     auto deps = scene_reader.get_dependencies();
     REQUIRE( deps.resources.size() == 2 );
     REQUIRE( deps.resources[ 0 ].name == "test_package1" );
     REQUIRE( deps.resources[ 0 ].elements.size() == 2 );
     REQUIRE( deps.resources[ 0 ].elements[ 0 ] == "test_texture" );
     REQUIRE( deps.resources[ 0 ].elements[ 1 ] == "test_shader" );
     REQUIRE( deps.resources[ 1 ].name == "test_package2" );
     REQUIRE( deps.resources[ 1 ].elements.size() == 1 );
     REQUIRE( deps.resources[ 1 ].elements[ 0 ] == "test_schema" );
     REQUIRE( deps.libs.size() == 1 );
     REQUIRE( deps.libs[ 0 ].name == "test_lib" );
     REQUIRE( deps.libs[ 0 ].elements.size() == 2 );
     REQUIRE( deps.libs[ 0 ].elements[ 0 ] == "test_setup" );
     REQUIRE( deps.libs[ 0 ].elements[ 1 ] == "test_loop" );
     REQUIRE( deps.pieces.size() == 0 );

     auto setup_func = scene_reader.get_setup_func();
     REQUIRE( setup_func.has_value() );
     REQUIRE( setup_func.value().package == 0 );
     REQUIRE( setup_func.value().resource == 0 );

     auto loop_func = scene_reader.get_loop_func();
     REQUIRE( loop_func.has_value() );
     REQUIRE( loop_func.value().package == 0 );
     REQUIRE( loop_func.value().resource == 1 );

     REQUIRE( scene_reader.get_scene_piece_count() == 1 );
     REQUIRE( !scene_reader.is_empty() );

     auto& piece_reader = scene_reader.get_scene_piece_reader( 0 );
     REQUIRE( piece_reader.is_empty() == false );

     auto& node_reader1 = piece_reader.get_current_node_reader();
     REQUIRE( piece_reader.get_current_node_type() == _16nar::tools::constructor2d::NodeType::Node2D );
     const auto pos = node_reader1.get_vec2f( "pos" ).value();
     REQUIRE( std::fabs( pos.x() - 10.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( pos.y() - -50.0f ) <= TEST_PRECISION );
     const auto scale1 = node_reader1.get_vec2f( "scale" ).value();
     REQUIRE( std::fabs( scale1.x() - 1.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( scale1.y() - 2.0f ) <= TEST_PRECISION );
     REQUIRE( piece_reader.next_node() == false );

     auto& state_reader = scene_reader.get_current_state_reader();
     REQUIRE( !state_reader.is_empty() );
     REQUIRE( state_reader.get_rendering() == true );
     REQUIRE( state_reader.get_updating() == false );

     auto& render_system_reader = state_reader.get_render_system_reader();
     REQUIRE( state_reader.get_render_system_type()
          == _16nar::tools::constructor2d::RenderSystemType::QTreeRenderSystem );
     const auto rs_size = render_system_reader.get_vec2i( "size" ).value();
     REQUIRE( rs_size == _16nar::Vec2i{ 5, 5 } );
     const auto quad_start = render_system_reader.get_vec2f( "quad_start" ).value();
     REQUIRE( std::fabs( quad_start.x() ) <= TEST_PRECISION );
     REQUIRE( std::fabs( quad_start.y() ) <= TEST_PRECISION );
     const auto quad_size = render_system_reader.get_vec2f( "quad_size" ).value();
     REQUIRE( std::fabs( quad_size.x() - 1000.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( quad_size.y() - 1000.0f ) <= TEST_PRECISION );

     auto& node_reader2 = state_reader.get_current_node_reader();
     REQUIRE( state_reader.get_current_node_type() == _16nar::tools::constructor2d::NodeType::Node2D );
     const auto scale2 = node_reader2.get_vec2f( "scale" ).value();
     REQUIRE( std::fabs( scale2.x() - 3.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( scale2.y() - 3.0f ) <= TEST_PRECISION );
     const auto link = node_reader2.get_resource_index( "link" ).value();
     REQUIRE( link.package == _16nar::tools::internal_package_num );
     REQUIRE( link.resource == 0 );
     REQUIRE( state_reader.next_node() == true );

     auto& node_reader3 = state_reader.get_current_node_reader();
     REQUIRE( state_reader.get_current_node_type() == _16nar::tools::constructor2d::NodeType::SpriteNode );
     const auto parent = node_reader3.get_uint32( "parent" ).value();
     REQUIRE( parent == 0 );
     const auto size = node_reader3.get_vec2f( "size" ).value();
     REQUIRE( std::fabs( size.x() - 10.0f ) <= TEST_PRECISION );
     REQUIRE( std::fabs( size.y() - 10.0f ) <= TEST_PRECISION );
     const auto texture = node_reader3.get_resource_index( "texture" ).value();
     REQUIRE( texture.package == 0 );
     REQUIRE( texture.resource == 0 );
     const auto shader = node_reader3.get_resource_index( "shader" ).value();
     REQUIRE( shader.package == 0 );
     REQUIRE( shader.resource == 1 );
     const auto name = node_reader3.get_string( "name" ).value();
     REQUIRE( name == "test_sprite" );
     REQUIRE( state_reader.next_node() == false );
}

} // anonymous namespace
