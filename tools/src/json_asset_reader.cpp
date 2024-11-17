#include <16nar/tools/json_asset_reader.h>

#include <16nar/tools/json_utils.inl>
#include <16nar/tools/binary_utils.h>

#include <stb_image.h>

#include <string>
#include <array>
#include <stdexcept>


namespace
{

void read_texture( const nlohmann::json& json, const std::string& in_dir,
     _16nar::tools::ResourceData& resource )
{
     _16nar::LoadParams< _16nar::ResourceType::Texture > params{};
     params.format = json.at( "format" ).template get< _16nar::BufferDataFormat >();
     params.min_filter = json.at( "min_filter" ).template get< _16nar::TextureFilter >();
     params.mag_filter = json.at( "mag_filter" ).template get< _16nar::TextureFilter >();
     params.wrap_x = json.at( "wrap_x" ).template get< _16nar::TextureWrap >();
     params.wrap_y = json.at( "wrap_y" ).template get< _16nar::TextureWrap >();
     params.data_type = json.at( "data_type" ).template get< _16nar::DataType >();
     params.samples = json.at( "samples" );
     bool raw = json.at( "raw" );

     std::array< int, _16nar::Vec2i::size > size = json.at( "size" );
     params.size.x() = size[ 0 ];
     params.size.y() = size[ 1 ];

     std::array< float, _16nar::Vec4f::size > border_color = json.at( "border_color" );
     params.border_color.x() = border_color[ 0 ];
     params.border_color.y() = border_color[ 1 ];
     params.border_color.z() = border_color[ 2 ];
     params.border_color.w() = border_color[ 3 ];

     int channels = 0;
     int required_channels = _16nar::tools::get_channel_count( params.format );
     std::size_t data_size = 0;
     std::string file = _16nar::tools::correct_path( in_dir, json.at( "file" ) );
     if ( raw )
     {
          params.data = _16nar::tools::read_binary( file, data_size );
     }
     else
     {
          std::byte *data = nullptr;
          if ( params.data_type == _16nar::DataType::Byte )
          {
               data = reinterpret_cast< std::byte * >(
                    stbi_load( file.c_str(), &params.size.x(), &params.size.y(),
                    &channels, required_channels ) );
               data_size = params.size.x() * params.size.y() * required_channels * sizeof( std::byte );
          }
          else
          {
               data = reinterpret_cast< std::byte * >(
                    stbi_loadf( file.c_str(), &params.size.x(), &params.size.y(),
                    &channels, required_channels ) );
               data_size = params.size.x() * params.size.y() * required_channels * sizeof( float );
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
     resource.data_sizes.emplace_back( static_cast< uint32_t >( data_size ) );
     resource.type = _16nar::ResourceType::Texture;
}


void read_shader( const nlohmann::json& json, const std::string& in_dir,
     _16nar::tools::ResourceData& resource )
{
     _16nar::LoadParams< _16nar::ResourceType::Shader > params{};
     const auto& shaders = json.at( "shaders" );
     for ( const auto& json_param : shaders )
     {
          _16nar::LoadParams< _16nar::ResourceType::Shader >::ShaderParams param{};
          param.entrypoint = json_param.at( "entrypoint" );
          param.type = json_param.at( "type" ). template get< _16nar::ShaderType >();
          param.from_source = json_param.at( "from_source" );

          std::string file = _16nar::tools::correct_path( in_dir, json_param.at( "file" ) );
          std::size_t data_size = 0;
          param.data = _16nar::tools::read_binary( file, param.size, param.from_source );

          params.shaders.emplace_back( param );
          resource.data_sizes.emplace_back( static_cast< uint32_t >( param.size ) );
     }
     resource.params = std::any{ params };
     resource.type = _16nar::ResourceType::Shader;
}


void read_vertex_buffer( const nlohmann::json& json, const std::string& in_dir,
     _16nar::tools::ResourceData& resource )
{
     _16nar::LoadParams< _16nar::ResourceType::VertexBuffer > params{};
     auto read_buffer_param = [ &in_dir ]( const auto& json, auto& param )
     {
          std::string file = _16nar::tools::correct_path( in_dir, json.at( "file" ) );
          param.data = _16nar::tools::read_binary( file, param.size );
          param.type = json.at( "type" ). template get< _16nar::BufferType >();
     };

     read_buffer_param( json[ "buffer" ], params.buffer );
     read_buffer_param( json[ "index_buffer" ], params.index_buffer );
     resource.data_sizes.emplace_back( static_cast< uint32_t >( params.buffer.size ) );
     resource.data_sizes.emplace_back( static_cast< uint32_t >( params.index_buffer.size ) );

     const auto& attributes = json.at( "attributes" );
     for ( const auto& json_attr : attributes )
     {
          _16nar::LoadParams< _16nar::ResourceType::VertexBuffer >::AttribParams param{};
          param.size = json_attr.at( "size" );
          param.data_type = json_attr.at( "data_type" ). template get< _16nar::DataType >();
          param.normalized = json_attr.at( "normalized" );

          params.attributes.emplace_back( param );
     }

     resource.params = std::any{ params };
     resource.type = _16nar::ResourceType::VertexBuffer;
}


void read_cubemap( const nlohmann::json& json, const std::string& in_dir,
     _16nar::tools::ResourceData& resource )
{
     _16nar::LoadParams< _16nar::ResourceType::Cubemap > params{};
     params.format = json.at( "format" ).template get< _16nar::BufferDataFormat >();
     params.min_filter = json.at( "min_filter" ).template get< _16nar::TextureFilter >();
     params.mag_filter = json.at( "mag_filter" ).template get< _16nar::TextureFilter >();
     params.wrap_x = json.at( "wrap_x" ).template get< _16nar::TextureWrap >();
     params.wrap_y = json.at( "wrap_y" ).template get< _16nar::TextureWrap >();
     params.wrap_z = json.at( "wrap_z" ).template get< _16nar::TextureWrap >();
     params.data_type = json.at( "data_type" ).template get< _16nar::DataType >();
     bool raw = json.at( "raw" );

     std::array< int, _16nar::Vec2i::size > size = json.at( "size" );
     params.size.x() = size[ 0 ];
     params.size.y() = size[ 1 ];

     std::array< float, _16nar::Vec4f::size > border_color = json.at( "border_color" );
     params.border_color.x() = border_color[ 0 ];
     params.border_color.y() = border_color[ 1 ];
     params.border_color.z() = border_color[ 2 ];
     params.border_color.w() = border_color[ 3 ];

     const auto& files = json.at( "files" );
     std::size_t file_num = 0;
     for ( const std::string& path : files )
     {
          std::string file = _16nar::tools::correct_path( in_dir, path );
          std::size_t data_size = 0;
          if ( raw )
          {
               params.data[ file_num ] = _16nar::tools::read_binary( file, data_size );
          }
          else
          {
               int channels = 0;
               int required_channels = _16nar::tools::get_channel_count( params.format );
               std::byte *data = nullptr;
               if ( params.data_type == _16nar::DataType::Byte )
               {
                    data = reinterpret_cast< std::byte * >(
                         stbi_load( file.c_str(), &params.size.x(), &params.size.y(),
                         &channels, required_channels ) );
                    data_size = params.size.x() * params.size.y() * required_channels * sizeof( std::byte );
               }
               else
               {
                    data = reinterpret_cast< std::byte * >(
                         stbi_loadf( file.c_str(), &params.size.x(), &params.size.y(),
                         &channels, required_channels ) );
                    data_size = params.size.x() * params.size.y() * required_channels * sizeof( float );
               }

               if ( !data )
               {
                    throw std::runtime_error{ "cannot read image from file " + file };
               }
               params.data[ file_num ] =
                    _16nar::DataSharedPtr{ new std::byte[ data_size ], std::default_delete< std::byte[] >() };
               std::memcpy( params.data[ file_num ].get(), data, data_size );
               stbi_image_free( data );
          }
          resource.data_sizes.emplace_back( static_cast< uint32_t >( data_size ) );
          ++file_num;
     }

     resource.params = std::any{ params };
     resource.type = _16nar::ResourceType::Cubemap;
}


_16nar::tools::ResourceData read_resource( const nlohmann::json& json, const std::string& in_dir )
{
     _16nar::tools::ResourceData resource{};
     _16nar::ResourceType type = json.at( "type" ).template get< _16nar::ResourceType >();
     resource.name = json.at( "name" );
     switch ( type )
     {
          case _16nar::ResourceType::Texture:
               read_texture( json, in_dir, resource );
               break;
          case _16nar::ResourceType::Shader:
               read_shader( json, in_dir, resource );
               break;
          case _16nar::ResourceType::VertexBuffer:
               read_vertex_buffer( json, in_dir, resource );
               break;
          case _16nar::ResourceType::Cubemap:
               read_cubemap( json, in_dir, resource );
               break;
          default:
               throw std::runtime_error{ "wrong resource type: "
                    + std::to_string( static_cast< std::size_t >( type ) ) };
     }
     return resource;
}

} // anonymous namespace


namespace _16nar::tools
{

JsonAssetReader::JsonAssetReader( const std::string& in_dir ):
     in_dir_{ in_dir }
{}


ResourceData JsonAssetReader::read_asset( std::istream& input )
{
     auto json = nlohmann::json::parse( input );
     return read_resource( json, in_dir_ );
}


PackageData JsonAssetReader::read_package( std::istream& input )
{
     PackageData package{};
     auto json = nlohmann::json::parse( input );
     const auto& resources = json[ "resources" ];
     for ( const nlohmann::json& resource : resources )
     {
          package.resources.emplace_back( read_resource( resource, in_dir_ ) );
     }
     return package;
}

} // namespace _16nar::tools
