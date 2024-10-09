#include <json_asset_reader.h>

#include <16nar/render/render_defs.h>

#include <stb_image.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <string>
#include <array>
#include <stdexcept>

namespace _16nar
{

NLOHMANN_JSON_SERIALIZE_ENUM( ResourceType, {
     { ResourceType::Unknown,      nullptr },
     { ResourceType::Texture,      "texture" },
     { ResourceType::Shader,       "shader" },
     { ResourceType::VertexBuffer, "vertex_buffer" },
     { ResourceType::Cubemap,      "cubemap" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( TextureWrap, {
     { TextureWrap::Repeat,             "repeat" },
     { TextureWrap::MirroredRepeat,     "mirrored_repeat" },
     { TextureWrap::ClampToEdge,        "clamp_to_edge" },
     { TextureWrap::ClampToBorder,      "clamp_to_border" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( TextureFilter, {
     { TextureFilter::Nearest,               "nearest" },
     { TextureFilter::Linear,                "linear" },
     { TextureFilter::NearestMipmapNearest,  "nearest_mipmap_nearest" },
     { TextureFilter::NearestMipmapLinear,   "nearest_mipmap_linear" },
     { TextureFilter::LinearMipmapNearest,   "linear_mipmap_nearest" },
     { TextureFilter::LinearMipmapLinear,    "linear_mipmap_linear" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( BufferDataFormat, {
     { BufferDataFormat::Rgb,           "rgb" },
     { BufferDataFormat::Rgba,          "rgba" },
     { BufferDataFormat::Srgb,          "srgb" },
     { BufferDataFormat::Srgba,         "srgba" },
     { BufferDataFormat::Depth,         "depth" },
     { BufferDataFormat::DepthStencil,  "depth_stencil" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( DataType, {
     { DataType::Byte,    "byte" },
     { DataType::Float,   "float" },
} )

} // namespace _16nar


namespace
{

void read_texture( nlohmann::json& json, _16nar::tools::ResourceData& resource )
{
     _16nar::LoadParams< _16nar::ResourceType::Texture > params;
     params.format = json.at( "format" ).template get< _16nar::BufferDataFormat >();
     params.min_filter = json.at( "min_filter" ).template get< _16nar::TextureFilter >();
     params.mag_filter = json.at( "mag_filter" ).template get< _16nar::TextureFilter >();
     params.wrap_x = json.at( "wrap_x" ).template get< _16nar::TextureWrap >();
     params.wrap_y = json.at( "wrap_y" ).template get< _16nar::TextureWrap >();
     params.data_type = json.at( "data_type" ).template get< _16nar::DataType >();
     params.samples = json.at( "samples" );
     bool binary = json.at( "binary" );

     std::array< int, _16nar::Vec2i::size > size = json.at( "size" );
     params.size.x() = size[ 0 ];
     params.size.y() = size[ 1 ];

     std::array< float, _16nar::Vec4f::size > border_color = json.at( "border_color" );
     params.border_color.x() = border_color[ 0 ];
     params.border_color.y() = border_color[ 1 ];
     params.border_color.z() = border_color[ 2 ];
     params.border_color.w() = border_color[ 3 ];

     int channels = 0;
     std::size_t data_size = 0;
     std::string file = json.at( "file" );
     if ( binary )
     {
          std::ifstream bin_file( file, std::ios_base::binary );
          bin_file.seekg( 0, std::ios_base::end );
          data_size = bin_file.tellg();
          bin_file.seekg( 0, std::ios_base::beg );
          params.data = _16nar::DataSharedPtr{ new std::byte[ data_size ], std::default_delete< std::byte[] >() };
          bin_file.read( reinterpret_cast< char * >( params.data.get() ), data_size );
     }
     else
     {
          std::byte *data = nullptr;
          if ( params.data_type == _16nar::DataType::Byte )
          {
               data = reinterpret_cast< std::byte * >(
                    stbi_load( file.c_str(), &params.size.x(), &params.size.y(), &channels, 0 ) );
               data_size = params.size.x() * params.size.y() * channels * sizeof( std::byte );
          }
          else
          {
               data = reinterpret_cast< std::byte * >(
                    stbi_loadf( file.c_str(), &params.size.x(), &params.size.y(), &channels, 0 ) );
               data_size = params.size.x() * params.size.y() * channels * sizeof( float );
          }

          if ( !data )
          {
               throw std::runtime_error{ "cannot read image from file " + file };
          }
          params.data = _16nar::DataSharedPtr{ new std::byte[ data_size ], std::default_delete< std::byte[] >() };
          std::memcpy( params.data.get(), data, data_size );
          stbi_image_free( data );
     }

     resource.params = std::any{ params };
     resource.name = json.at( "name" );
     resource.data_sizes = std::vector< uint32_t >{ static_cast< uint32_t >( data_size ) };
     resource.type = _16nar::ResourceType::Texture;
}

} // anonymous namespace


namespace _16nar::tools
{

void JsonAssetReader::read_asset( std::istream& file, _16nar::tools::ResourceData& resource )
{
     auto json = nlohmann::json::parse( file );
     ResourceType type = json.at( "type" ).template get< ResourceType >();
     switch ( type )
     {
          case ResourceType::Texture:
               read_texture( json, resource );
               break;
          default:
               throw std::runtime_error{ "wrong resource type: "
                    + std::to_string( static_cast< std::size_t >( type ) ) };
     }
}

} // namespace _16nar::tools
