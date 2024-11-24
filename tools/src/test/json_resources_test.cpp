#include <catch2/catch_test_macros.hpp>

#include <16nar/tools/json_asset_reader.h>
#include <16nar/tools/json_asset_writer.h>
#include <16nar/render/render_defs.h>

#include <nlohmann/json.hpp>

#include <fstream>
#include <filesystem>
#include <vector>
#include <stdexcept>

namespace fs = std::filesystem;

namespace
{

TEST_CASE( "Textures reading and writing in JSON format", "[json_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader reader{ "data" };
     _16nar::tools::JsonAssetWriter writer{ "data/out" };

     std::ifstream ifs{ "data/test_texture.json" };
     _16nar::tools::ResourceData data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( data.type == _16nar::ResourceType::Texture );
     REQUIRE( data.name == "test_texture" );
     REQUIRE( data.data_sizes.size() == 1 );
     REQUIRE( data.data_sizes.at( 0 ) == 64 * 64 * 4 * sizeof( std::byte ) );

     auto tex_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Texture > >( data.params );
     REQUIRE( tex_data.format == _16nar::BufferDataFormat::Rgba );
     REQUIRE( tex_data.min_filter == _16nar::TextureFilter::Nearest );
     REQUIRE( tex_data.mag_filter == _16nar::TextureFilter::NearestMipmapLinear );
     REQUIRE( tex_data.wrap_x == _16nar::TextureWrap::Repeat );
     REQUIRE( tex_data.wrap_y == _16nar::TextureWrap::ClampToEdge );
     REQUIRE( tex_data.data_type == _16nar::DataType::Byte );
     REQUIRE( tex_data.samples == 0 );
     REQUIRE( tex_data.size == _16nar::Vec2i{ 64, 64 } );
     REQUIRE( tex_data.border_color == _16nar::Vec4f{ 0.8f, 0.24f, 0.55f, 1.0f } );

     std::ofstream ofs{ "data/out/" + data.name + "_out." + writer.get_file_ext() };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs_written{ "data/out/test_texture_out.json" };
     auto written = nlohmann::json::parse( ifs_written );
     REQUIRE( written[ "type" ] == "texture" );
     REQUIRE( written[ "name" ] == "test_texture" );
     REQUIRE( written[ "format" ] == "rgba" );
     REQUIRE( written[ "min_filter" ] == "nearest" );
     REQUIRE( written[ "mag_filter" ] == "nearest_mipmap_linear" );
     REQUIRE( written[ "wrap_x" ] == "repeat" );
     REQUIRE( written[ "wrap_y" ] == "clamp_to_edge" );
     REQUIRE( written[ "data_type" ] == "byte" );
     REQUIRE( written[ "raw" ] == true );
     REQUIRE( written[ "file" ] == "test_texture_texture.bin" );
}


TEST_CASE( "Shaders reading and writing in JSON format", "[json_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader reader{ "data" };
     _16nar::tools::JsonAssetWriter writer{ "data/out" };

     std::ifstream ifs{ "data/test_shader.json" };
     _16nar::tools::ResourceData data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( data.type == _16nar::ResourceType::Shader );
     REQUIRE( data.name == "test_shader" );
     REQUIRE( data.data_sizes.size() == 2 );
     REQUIRE( data.data_sizes.at( 0 ) == 125 * sizeof( std::byte ) );
     REQUIRE( data.data_sizes.at( 1 ) == 102 * sizeof( std::byte ) );

     auto sh_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Shader > >( data.params );
     REQUIRE( sh_data.shaders.size() == 2 );
     const auto& vtx_shader = sh_data.shaders.at( 0 );
     const auto& fr_shader = sh_data.shaders.at( 1 );

     REQUIRE( vtx_shader.entrypoint == "main" );
     REQUIRE( vtx_shader.size == 125 * sizeof( std::byte ) );
     REQUIRE( vtx_shader.type == _16nar::ShaderType::Vertex );
     REQUIRE( vtx_shader.from_source == true );

     REQUIRE( fr_shader.entrypoint == "main" );
     REQUIRE( fr_shader.size == 102 * sizeof( std::byte ) );
     REQUIRE( fr_shader.type == _16nar::ShaderType::Fragment );
     REQUIRE( fr_shader.from_source == true );

     std::ofstream ofs{ "data/out/" + data.name + "_out." + writer.get_file_ext() };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs_written{ "data/out/test_shader_out.json" };
     auto written = nlohmann::json::parse( ifs_written );
     REQUIRE( written[ "type" ] == "shader" );
     REQUIRE( written[ "name" ] == "test_shader" );

     std::vector< nlohmann::json > shaders = written[ "shaders" ];
     REQUIRE( shaders.size() == 2 );
     const auto& vtx_written = shaders.at( 0 );
     const auto& fr_written = shaders.at( 1 );

     REQUIRE( vtx_written[ "entrypoint" ] == "main" );
     REQUIRE( vtx_written[ "type" ] == "vertex" );
     REQUIRE( vtx_written[ "file" ] == "test_shader_shader0.bin" );
     REQUIRE( vtx_written[ "from_source" ] == true );

     REQUIRE( fr_written[ "entrypoint" ] == "main" );
     REQUIRE( fr_written[ "type" ] == "fragment" );
     REQUIRE( fr_written[ "file" ] == "test_shader_shader1.bin" );
     REQUIRE( fr_written[ "from_source" ] == true );
}


TEST_CASE( "Vertex buffers reading and writing in JSON format", "[json_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader reader{ "data" };
     _16nar::tools::JsonAssetWriter writer{ "data/out" };

     std::ifstream ifs{ "data/test_vertex_buffer.json" };
     _16nar::tools::ResourceData data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( data.type == _16nar::ResourceType::VertexBuffer );
     REQUIRE( data.name == "test_vertex_buffer" );
     REQUIRE( data.data_sizes.size() == 2 );
     REQUIRE( data.data_sizes.at( 0 ) == 16 );
     REQUIRE( data.data_sizes.at( 1 ) == 16 );

     auto vb_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::VertexBuffer > >( data.params );
     REQUIRE( vb_data.attributes.size() == 2 );
     REQUIRE( vb_data.attributes[ 0 ].size == 2 );
     REQUIRE( vb_data.attributes[ 0 ].data_type == _16nar::DataType::Float );
     REQUIRE( vb_data.attributes[ 0 ].normalized == true );
     REQUIRE( vb_data.attributes[ 1 ].size == 2 );
     REQUIRE( vb_data.attributes[ 1 ].data_type == _16nar::DataType::Float );
     REQUIRE( vb_data.attributes[ 1 ].normalized == false );
     REQUIRE( vb_data.buffer.size == 16 );
     REQUIRE( vb_data.buffer.type == _16nar::BufferType::DynamicDraw );
     REQUIRE( vb_data.index_buffer.size == 16 );
     REQUIRE( vb_data.index_buffer.type == _16nar::BufferType::StaticDraw );

     std::ofstream ofs{ "data/out/" + data.name + "_out." + writer.get_file_ext() };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs_written{ "data/out/test_vertex_buffer_out.json" };
     auto written = nlohmann::json::parse( ifs_written );
     REQUIRE( written[ "type" ] == "vertex_buffer" );
     REQUIRE( written[ "name" ] == "test_vertex_buffer" );
     REQUIRE( written[ "name" ] == "test_vertex_buffer" );
     REQUIRE( written[ "buffer" ][ "file" ] == "test_vertex_buffer_buffer.bin" );
     REQUIRE( written[ "buffer" ][ "type" ] == "dynamic_draw" );
     REQUIRE( written[ "index_buffer" ][ "file" ] == "test_vertex_buffer_index_buffer.bin" );
     REQUIRE( written[ "index_buffer" ][ "type" ] == "static_draw" );

     std::vector< nlohmann::json > attributes = written[ "attributes" ];
     REQUIRE( attributes.size() == 2 );
     REQUIRE( attributes[ 0 ][ "data_type" ] == "float" );
     REQUIRE( attributes[ 0 ][ "size" ] == 2 );
     REQUIRE( attributes[ 0 ][ "normalized" ] == true );
     REQUIRE( attributes[ 1 ][ "data_type" ] == "float" );
     REQUIRE( attributes[ 1 ][ "size" ] == 2 );
     REQUIRE( attributes[ 1 ][ "normalized" ] == false );
}


TEST_CASE( "Cubemaps reading and writing in JSON format", "[json_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader reader{ "data" };
     _16nar::tools::JsonAssetWriter writer{ "data/out" };

     std::ifstream ifs{ "data/test_cubemap.json" };
     _16nar::tools::ResourceData data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( data.type == _16nar::ResourceType::Cubemap );
     REQUIRE( data.name == "test_cubemap" );
     REQUIRE( data.data_sizes.size() == 6 );
     REQUIRE( data.data_sizes.at( 0 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( data.data_sizes.at( 1 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( data.data_sizes.at( 2 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( data.data_sizes.at( 3 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( data.data_sizes.at( 4 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( data.data_sizes.at( 5 ) == 64 * 64 * 3 * sizeof( std::byte ) );

     auto cm_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Cubemap > >( data.params );
     REQUIRE( cm_data.format == _16nar::BufferDataFormat::Rgb );
     REQUIRE( cm_data.min_filter == _16nar::TextureFilter::NearestMipmapLinear );
     REQUIRE( cm_data.mag_filter == _16nar::TextureFilter::Nearest );
     REQUIRE( cm_data.wrap_x == _16nar::TextureWrap::Repeat );
     REQUIRE( cm_data.wrap_y == _16nar::TextureWrap::Repeat );
     REQUIRE( cm_data.wrap_z == _16nar::TextureWrap::ClampToEdge );
     REQUIRE( cm_data.data_type == _16nar::DataType::Byte );
     REQUIRE( cm_data.size == _16nar::Vec2i{ 64, 64 } );
     REQUIRE( cm_data.border_color == _16nar::Vec4f{ 0.8f, 0.24f, 0.55f, 1.0f } );

     std::ofstream ofs{ "data/out/" + data.name + "_out." + writer.get_file_ext() };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs_written{ "data/out/test_cubemap_out.json" };
     auto written = nlohmann::json::parse( ifs_written );
     REQUIRE( written[ "type" ] == "cubemap" );
     REQUIRE( written[ "name" ] == "test_cubemap" );
     REQUIRE( written[ "format" ] == "rgb" );
     REQUIRE( written[ "min_filter" ] == "nearest_mipmap_linear" );
     REQUIRE( written[ "mag_filter" ] == "nearest" );
     REQUIRE( written[ "wrap_x" ] == "repeat" );
     REQUIRE( written[ "wrap_y" ] == "repeat" );
     REQUIRE( written[ "wrap_z" ] == "clamp_to_edge" );
     REQUIRE( written[ "data_type" ] == "byte" );
     REQUIRE( written[ "raw" ] == true );

     std::vector< std::string > files = written[ "files" ];
     REQUIRE( files.size() == 6 );
     REQUIRE( files.at( 0 ) == "test_cubemap_cubemap0.bin" );
     REQUIRE( files.at( 1 ) == "test_cubemap_cubemap1.bin" );
     REQUIRE( files.at( 2 ) == "test_cubemap_cubemap2.bin" );
     REQUIRE( files.at( 3 ) == "test_cubemap_cubemap3.bin" );
     REQUIRE( files.at( 4 ) == "test_cubemap_cubemap4.bin" );
     REQUIRE( files.at( 5 ) == "test_cubemap_cubemap5.bin" );
}


TEST_CASE( "Packages reading and writing in JSON format", "[json_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader reader{ "data" };
     _16nar::tools::JsonAssetWriter writer{ "data/out" };

     std::ifstream ifs{ "data/test_package.json" };
     _16nar::tools::PackageData pkg = reader.read_package( ifs );
     ifs.close();

     REQUIRE( pkg.resources.size() == 2 );

     const auto& data0 = pkg.resources.at( 0 );
     REQUIRE( data0.type == _16nar::ResourceType::Texture );
     REQUIRE( data0.name == "test_pkg_texture" );
     REQUIRE( data0.data_sizes.size() == 1 );
     REQUIRE( data0.data_sizes.at( 0 ) == 64 * 64 * 4 * sizeof( std::byte ) );

     auto tex_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Texture > >( data0.params );
     REQUIRE( tex_data.format == _16nar::BufferDataFormat::Rgba );
     REQUIRE( tex_data.min_filter == _16nar::TextureFilter::Nearest );
     REQUIRE( tex_data.mag_filter == _16nar::TextureFilter::NearestMipmapLinear );
     REQUIRE( tex_data.wrap_x == _16nar::TextureWrap::Repeat );
     REQUIRE( tex_data.wrap_y == _16nar::TextureWrap::ClampToEdge );
     REQUIRE( tex_data.data_type == _16nar::DataType::Byte );
     REQUIRE( tex_data.samples == 0 );
     REQUIRE( tex_data.size == _16nar::Vec2i{ 64, 64 } );
     REQUIRE( tex_data.border_color == _16nar::Vec4f{ 0.8f, 0.24f, 0.55f, 1.0f } );

     const auto& data1 = pkg.resources.at( 1 );
     REQUIRE( data1.type == _16nar::ResourceType::Shader );
     REQUIRE( data1.name == "test_pkg_shader" );
     REQUIRE( data1.data_sizes.size() == 2 );
     REQUIRE( data1.data_sizes.at( 0 ) == 125 * sizeof( std::byte ) );
     REQUIRE( data1.data_sizes.at( 1 ) == 102 * sizeof( std::byte ) );

     auto sh_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Shader > >( data1.params );
     REQUIRE( sh_data.shaders.size() == 2 );
     const auto& vtx_shader = sh_data.shaders.at( 0 );
     const auto& fr_shader = sh_data.shaders.at( 1 );

     REQUIRE( vtx_shader.entrypoint == "main" );
     REQUIRE( vtx_shader.size == 125 * sizeof( std::byte ) );
     REQUIRE( vtx_shader.type == _16nar::ShaderType::Vertex );
     REQUIRE( vtx_shader.from_source == true );

     REQUIRE( fr_shader.entrypoint == "main" );
     REQUIRE( fr_shader.size == 102 * sizeof( std::byte ) );
     REQUIRE( fr_shader.type == _16nar::ShaderType::Fragment );
     REQUIRE( fr_shader.from_source == true );

     std::ofstream ofs{ std::string{ "data/out/test_package_out." } + writer.get_pkg_ext() };
     writer.write_package( ofs, pkg );
     ofs.close();

     std::ifstream ifs_written{ "data/out/test_package_out.json" };
     auto pkg_written = nlohmann::json::parse( ifs_written );
     auto written = pkg_written[ "resources" ][ 0 ];
     REQUIRE( written[ "type" ] == "texture" );
     REQUIRE( written[ "name" ] == "test_pkg_texture" );
     REQUIRE( written[ "format" ] == "rgba" );
     REQUIRE( written[ "min_filter" ] == "nearest" );
     REQUIRE( written[ "mag_filter" ] == "nearest_mipmap_linear" );
     REQUIRE( written[ "wrap_x" ] == "repeat" );
     REQUIRE( written[ "wrap_y" ] == "clamp_to_edge" );
     REQUIRE( written[ "data_type" ] == "byte" );
     REQUIRE( written[ "raw" ] == true );
     REQUIRE( written[ "file" ] == "test_pkg_texture_texture.bin" );

     written = pkg_written[ "resources" ][ 1 ];
     REQUIRE( written[ "type" ] == "shader" );
     REQUIRE( written[ "name" ] == "test_pkg_shader" );

     std::vector< nlohmann::json > shaders = written[ "shaders" ];
     REQUIRE( shaders.size() == 2 );
     const auto& vtx_written = shaders.at( 0 );
     const auto& fr_written = shaders.at( 1 );

     REQUIRE( vtx_written[ "entrypoint" ] == "main" );
     REQUIRE( vtx_written[ "type" ] == "vertex" );
     REQUIRE( vtx_written[ "file" ] == "test_pkg_shader_shader0.bin" );
     REQUIRE( vtx_written[ "from_source" ] == true );

     REQUIRE( fr_written[ "entrypoint" ] == "main" );
     REQUIRE( fr_written[ "type" ] == "fragment" );
     REQUIRE( fr_written[ "file" ] == "test_pkg_shader_shader1.bin" );
     REQUIRE( fr_written[ "from_source" ] == true );
}

}
