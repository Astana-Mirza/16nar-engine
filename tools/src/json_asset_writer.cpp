#include <16nar/tools/json_asset_writer.h>

#include <16nar/tools/json_utils.inl>
#include <16nar/tools/utils.h>

#include <string>
#include <array>
#include <stdexcept>

namespace
{

constexpr char raw_ext[] = ".bin";


void write_texture( const _16nar::tools::ResourceData& resource,
     const std::string& out_dir, nlohmann::json& json )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Texture > >( resource.params );

     json[ "format" ] = params.format;
     json[ "min_filter" ] = params.min_filter;
     json[ "mag_filter" ] = params.mag_filter;
     json[ "wrap_x" ] = params.wrap_x;
     json[ "wrap_y" ] = params.wrap_y;
     json[ "data_type" ] = params.data_type;
     json[ "samples" ] = params.samples;
     json[ "raw" ] = true;    // always raw, because we don't know desired format

     std::array< int, _16nar::Vec2i::size > size{ params.size.x(), params.size.y() };
     json[ "size" ] = size;

     std::array< float, _16nar::Vec4f::size > border_color{
          params.border_color.x(),
          params.border_color.y(),
          params.border_color.z(),
          params.border_color.w()
     };
     json[ "border_color" ] = border_color;

     std::string file = resource.name + "_texture" + raw_ext;
     std::string path = _16nar::tools::correct_path( out_dir, file );
     json[ "file" ] = file;
     _16nar::tools::write_binary( path, params.data, resource.data_sizes.at( 0 ) );
}


void write_shader( const _16nar::tools::ResourceData& resource,
     const std::string& out_dir, nlohmann::json& json )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Shader > >( resource.params );

     std::size_t shader_num = 0;
     auto shaders = nlohmann::json::array();
     for ( const auto& param : params.shaders )
     {
          nlohmann::json shader{};
          shader[ "entrypoint" ] = param.entrypoint;
          shader[ "type" ] = param.type;
          shader[ "from_source" ] = param.from_source;

          std::string file = resource.name + "_shader" + std::to_string( shader_num ) + raw_ext;
          std::string path = _16nar::tools::correct_path( out_dir, file );
          shader[ "file" ] = file;
          _16nar::tools::write_binary( path, param.data,
               resource.data_sizes.at( shader_num ), param.from_source );

          shaders.push_back( shader );
          ++shader_num;
     }
     json[ "shaders" ] = shaders;
}


void write_vertex_buffer( const _16nar::tools::ResourceData& resource,
     const std::string& out_dir, nlohmann::json& json )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::VertexBuffer > >( resource.params );

     nlohmann::json buffer{};
     std::string file = resource.name + "_buffer" + raw_ext;
     std::string path = _16nar::tools::correct_path( out_dir, file );
     buffer[ "type" ] = params.buffer.type;
     buffer[ "file" ] = file;
     _16nar::tools::write_binary( path, params.buffer.data, params.buffer.size );
     json[ "buffer" ] = buffer;

     nlohmann::json index_buffer{};
     file = resource.name + "_index_buffer" + raw_ext;
     path = _16nar::tools::correct_path( out_dir, file );
     index_buffer[ "type" ] = params.index_buffer.type;
     index_buffer[ "file" ] = file;
     _16nar::tools::write_binary( path, params.index_buffer.data, params.index_buffer.size );
     json[ "index_buffer" ] = index_buffer;

     auto attributes = nlohmann::json::array();
     for ( std::size_t i = 0; i < params.attributes.size(); i++ )
     {
          nlohmann::json attr{};
          attr[ "data_type" ] = params.attributes.at( i ).data_type;
          attr[ "size" ] = params.attributes.at( i ).size;
          attr[ "normalized" ] = params.attributes.at( i ).normalized;
          attributes.push_back( attr );
     }
     json[ "attributes" ] = attributes;
}


void write_cubemap( const _16nar::tools::ResourceData& resource,
     const std::string& out_dir, nlohmann::json& json )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Cubemap > >( resource.params );

     json[ "format" ] = params.format;
     json[ "min_filter" ] = params.min_filter;
     json[ "mag_filter" ] = params.mag_filter;
     json[ "wrap_x" ] = params.wrap_x;
     json[ "wrap_y" ] = params.wrap_y;
     json[ "wrap_z" ] = params.wrap_z;
     json[ "data_type" ] = params.data_type;
     json[ "raw" ] = true;    // always raw, because we don't know desired format

     std::array< int, _16nar::Vec2i::size > size{ params.size.x(), params.size.y() };
     json[ "size" ] = size;

     std::array< float, _16nar::Vec4f::size > border_color{
          params.border_color.x(),
          params.border_color.y(),
          params.border_color.z(),
          params.border_color.w()
     };
     json[ "border_color" ] = border_color;

     std::string file_prefix = resource.name + "_cubemap";
     std::array< std::string, 6 > files;
     for ( std::size_t i = 0; i < files.size(); i++ )
     {
          std::string file = file_prefix + std::to_string( i ) + raw_ext;
          std::string path = _16nar::tools::correct_path( out_dir, file );
          files[ i ] = file;
          _16nar::tools::write_binary( path, params.data.at( i ), resource.data_sizes.at( i ) );
     }
     json[ "files" ] = files;
}


nlohmann::json write_resource( const _16nar::tools::ResourceData& resource, const std::string& out_dir )
{
     nlohmann::json json{};
     switch ( resource.type )
     {
          case _16nar::ResourceType::Texture:
               write_texture( resource, out_dir, json );
               break;
          case _16nar::ResourceType::Shader:
               write_shader( resource, out_dir, json );
               break;
          case _16nar::ResourceType::VertexBuffer:
               write_vertex_buffer( resource, out_dir, json );
               break;
          case _16nar::ResourceType::Cubemap:
               write_cubemap( resource, out_dir, json );
               break;
          default:
               throw std::runtime_error{ "wrong resource type: "
                    + std::to_string( static_cast< std::size_t >( resource.type ) ) };
     }
     json[ "type" ] = resource.type;
     json[ "name" ] = resource.name;
     return json;
}

} // anonymous namespace


namespace _16nar::tools
{

JsonAssetWriter::JsonAssetWriter( const std::string& out_dir ):
     out_dir_{ out_dir }
{}


void JsonAssetWriter::write_asset( std::ostream& output, const ResourceData& resource )
{
     output << write_resource( resource, out_dir_ );
}


void JsonAssetWriter::write_package( std::ostream& output, const PackageData& package )
{
     nlohmann::json json{};
     auto resources = nlohmann::json::array();
     for ( const auto& resource : package.resources )
     {
          resources.push_back( write_resource( resource, out_dir_ ) );
     }
     json[ "resources" ] = resources;
     output << json;
}

} // namespace _16nar::tools
