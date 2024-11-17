#include <catch2/catch_test_macros.hpp>

#include <16nar/tools/json_asset_reader.h>
#include <16nar/tools/flatbuffers_asset_reader.h>
#include <16nar/tools/flatbuffers_asset_writer.h>
#include <16nar/render/render_defs.h>

#include <fstream>
#include <filesystem>
#include <vector>
#include <stdexcept>

namespace fs = std::filesystem;

namespace
{

TEST_CASE( "Textures reading and writing in flatbuffers format", "[flatbuffers_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader json_reader{ "data" };
     _16nar::tools::FlatBuffersAssetReader reader{};
     _16nar::tools::FlatBuffersAssetWriter writer{};

     std::ifstream json_ifs{ "data/test_texture.json" };
     _16nar::tools::ResourceData data = json_reader.read_asset( json_ifs );
     json_ifs.close();

     std::ofstream ofs{ "data/out/test_texture.narasset", std::ios::out | std::ios::binary };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs{ "data/out/test_texture.narasset", std::ios::in | std::ios::binary };
     _16nar::tools::ResourceData read_data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( read_data.type == _16nar::ResourceType::Texture );
     REQUIRE( read_data.data_sizes.size() == 1 );
     REQUIRE( read_data.data_sizes.at( 0 ) == 64 * 64 * 4 * sizeof( std::byte ) );
     REQUIRE( read_data.name == "test_texture" );

     auto tex_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Texture > >( read_data.params );
     REQUIRE( tex_data.format == _16nar::BufferDataFormat::Rgba );
     REQUIRE( tex_data.min_filter == _16nar::TextureFilter::Nearest );
     REQUIRE( tex_data.mag_filter == _16nar::TextureFilter::NearestMipmapLinear );
     REQUIRE( tex_data.wrap_x == _16nar::TextureWrap::Repeat );
     REQUIRE( tex_data.wrap_y == _16nar::TextureWrap::ClampToEdge );
     REQUIRE( tex_data.data_type == _16nar::DataType::Byte );
     REQUIRE( tex_data.samples == 0 );
     REQUIRE( tex_data.size == _16nar::Vec2i{ 64, 64 } );
     REQUIRE( tex_data.border_color == _16nar::Vec4f{ 0.8f, 0.24f, 0.55f, 1.0f } );
}


TEST_CASE( "Shaders reading and writing in flatbuffers format", "[flatbuffers_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader json_reader{ "data" };
     _16nar::tools::FlatBuffersAssetReader reader{};
     _16nar::tools::FlatBuffersAssetWriter writer{};

     std::ifstream json_ifs{ "data/test_shader.json" };
     _16nar::tools::ResourceData data = json_reader.read_asset( json_ifs );
     json_ifs.close();

     std::ofstream ofs{ "data/out/test_shader.narasset", std::ios::out | std::ios::binary };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs{ "data/out/test_shader.narasset", std::ios::in | std::ios::binary };
     _16nar::tools::ResourceData read_data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( read_data.type == _16nar::ResourceType::Shader );
     REQUIRE( read_data.data_sizes.size() == 2 );
     REQUIRE( read_data.data_sizes.at( 0 ) == 125 * sizeof( std::byte ) );
     REQUIRE( read_data.data_sizes.at( 1 ) == 102 * sizeof( std::byte ) );
     REQUIRE( read_data.name == "test_shader" );

     auto sh_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Shader > >( read_data.params );
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
}


TEST_CASE( "Vertex buffers reading and writing in flatbuffers format", "[flatbuffers_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader json_reader{ "data" };
     _16nar::tools::FlatBuffersAssetReader reader{};
     _16nar::tools::FlatBuffersAssetWriter writer{};

     std::ifstream json_ifs{ "data/test_vertex_buffer.json" };
     _16nar::tools::ResourceData data = json_reader.read_asset( json_ifs );
     json_ifs.close();

     std::ofstream ofs{ "data/out/test_vertex_buffer.narasset", std::ios::out | std::ios::binary };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs{ "data/out/test_vertex_buffer.narasset", std::ios::in | std::ios::binary };
     _16nar::tools::ResourceData read_data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( read_data.type == _16nar::ResourceType::VertexBuffer );
     REQUIRE( read_data.data_sizes.size() == 2 );
     REQUIRE( read_data.data_sizes.at( 0 ) == 16 );
     REQUIRE( read_data.data_sizes.at( 1 ) == 16 );
     REQUIRE( read_data.name == "test_vertex_buffer" );

     auto vb_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::VertexBuffer > >( read_data.params );
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
}


TEST_CASE( "Cubemaps reading and writing in flatbuffers format", "[flatbuffers_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader json_reader{ "data" };
     _16nar::tools::FlatBuffersAssetReader reader{};
     _16nar::tools::FlatBuffersAssetWriter writer{};

     std::ifstream json_ifs{ "data/test_cubemap.json" };
     _16nar::tools::ResourceData data = json_reader.read_asset( json_ifs );
     json_ifs.close();

     std::ofstream ofs{ "data/out/test_cubemap.narasset", std::ios::out | std::ios::binary };
     writer.write_asset( ofs, data );
     ofs.close();

     std::ifstream ifs{ "data/out/test_cubemap.narasset", std::ios::in | std::ios::binary };
     _16nar::tools::ResourceData read_data = reader.read_asset( ifs );
     ifs.close();

     REQUIRE( read_data.type == _16nar::ResourceType::Cubemap );
     REQUIRE( read_data.data_sizes.size() == 6 );
     REQUIRE( read_data.data_sizes.at( 0 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( read_data.data_sizes.at( 1 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( read_data.data_sizes.at( 2 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( read_data.data_sizes.at( 3 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( read_data.data_sizes.at( 4 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( read_data.data_sizes.at( 5 ) == 64 * 64 * 3 * sizeof( std::byte ) );
     REQUIRE( read_data.name == "test_cubemap" );

     auto cm_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Cubemap > >( read_data.params );
     REQUIRE( cm_data.format == _16nar::BufferDataFormat::Rgb );
     REQUIRE( cm_data.min_filter == _16nar::TextureFilter::NearestMipmapLinear );
     REQUIRE( cm_data.mag_filter == _16nar::TextureFilter::Nearest );
     REQUIRE( cm_data.wrap_x == _16nar::TextureWrap::Repeat );
     REQUIRE( cm_data.wrap_y == _16nar::TextureWrap::Repeat );
     REQUIRE( cm_data.wrap_z == _16nar::TextureWrap::ClampToEdge );
     REQUIRE( cm_data.data_type == _16nar::DataType::Byte );
     REQUIRE( cm_data.size == _16nar::Vec2i{ 64, 64 } );
     REQUIRE( cm_data.border_color == _16nar::Vec4f{ 0.8f, 0.24f, 0.55f, 1.0f } );
}


TEST_CASE( "Packages reading and writing in flatbuffers format", "[flatbuffers_resources]" )
{
     if ( !fs::exists( "data/out" ) && !fs::create_directories( "data/out" ) )
     {
          throw std::runtime_error{ "cannot create output directory" };
     }
     _16nar::tools::JsonAssetReader json_reader{ "data" };
     _16nar::tools::FlatBuffersAssetReader reader{};
     _16nar::tools::FlatBuffersAssetWriter writer{};

     std::ifstream json_ifs{ "data/test_package.json" };
     _16nar::tools::PackageData pkg = json_reader.read_package( json_ifs );
     json_ifs.close();

     std::ofstream ofs{ "data/out/test_package.nrs", std::ios::out | std::ios::binary };
     writer.write_package( ofs, pkg );
     ofs.close();

     std::ifstream ifs{ "data/out/test_package.nrs", std::ios::in | std::ios::binary };
     _16nar::tools::PackageData read_pkg = reader.read_package( ifs );
     ifs.close();

     REQUIRE( read_pkg.resources.size() == 2 );

     const auto& read_data0 = read_pkg.resources.at( 0 );
     REQUIRE( read_data0.type == _16nar::ResourceType::Texture );
     REQUIRE( read_data0.data_sizes.size() == 1 );
     REQUIRE( read_data0.data_sizes.at( 0 ) == 64 * 64 * 4 * sizeof( std::byte ) );
     REQUIRE( read_data0.name == "test_pkg_texture" );

     auto tex_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Texture > >( read_data0.params );
     REQUIRE( tex_data.format == _16nar::BufferDataFormat::Rgba );
     REQUIRE( tex_data.min_filter == _16nar::TextureFilter::Nearest );
     REQUIRE( tex_data.mag_filter == _16nar::TextureFilter::NearestMipmapLinear );
     REQUIRE( tex_data.wrap_x == _16nar::TextureWrap::Repeat );
     REQUIRE( tex_data.wrap_y == _16nar::TextureWrap::ClampToEdge );
     REQUIRE( tex_data.data_type == _16nar::DataType::Byte );
     REQUIRE( tex_data.samples == 0 );
     REQUIRE( tex_data.size == _16nar::Vec2i{ 64, 64 } );
     REQUIRE( tex_data.border_color == _16nar::Vec4f{ 0.8f, 0.24f, 0.55f, 1.0f } );

     const auto& read_data1 = read_pkg.resources.at( 1 );
     REQUIRE( read_data1.type == _16nar::ResourceType::Shader );
     REQUIRE( read_data1.data_sizes.size() == 2 );
     REQUIRE( read_data1.data_sizes.at( 0 ) == 125 * sizeof( std::byte ) );
     REQUIRE( read_data1.data_sizes.at( 1 ) == 102 * sizeof( std::byte ) );
     REQUIRE( read_data1.name == "test_pkg_shader" );

     auto sh_data = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Shader > >( read_data1.params );
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
}

}
