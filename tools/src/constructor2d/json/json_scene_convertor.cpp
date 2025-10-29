#include <16nar/tools/constructor2d/json/json_scene_convertor.h>

#include <16nar/tools/assets/json/json_props_writer_factory.h>
#include <16nar/tools/assets/json/json_props_writer.h>
#include <16nar/tools/assets/json/json_props_reader.h>
#include <16nar/tools/constructor2d/iscene_piece_reader.h>
#include <16nar/tools/constructor2d/iscene_state_reader.h>
#include <16nar/tools/constructor2d/schemas.h>
#include <16nar/tools/utils.h>

#include <nlohmann/json.hpp>

#include <array>
#include <fstream>
#include <stdexcept>

namespace _16nar::tools::constructor2d
{
namespace
{

void store_dependencies( const Dependencies& deps, nlohmann::json& json )
{
     nlohmann::json deps_json;
     auto resources = nlohmann::json::array();
     for ( const auto& res : deps.resources )
     {
          nlohmann::json dep_json;
          const auto& elements = res.elements;
          dep_json[ "name" ] = res.name;
          dep_json[ "elements" ] = elements;
          resources.push_back( dep_json );
     }
     deps_json[ "resources" ] = resources;
     auto libs = nlohmann::json::array();
     for ( const auto& res : deps.libs )
     {
          nlohmann::json dep_json;
          const auto& elements = res.elements;
          dep_json[ "name" ] = res.name;
          dep_json[ "elements" ] = elements;
          libs.push_back( dep_json );
     }
     deps_json[ "libs" ] = libs;
     auto pieces = nlohmann::json::array();
     for ( const auto& res : deps.pieces )
     {
          nlohmann::json dep_json;
          const auto& elements = res.elements;
          dep_json[ "name" ] = res.name;
          dep_json[ "elements" ] = elements;
          pieces.push_back( dep_json );
     }
     deps_json[ "pieces" ] = pieces;
     json[ "deps" ] = deps_json;
}


nlohmann::json convert_node( NodeType type, IPropsReader& reader )
{
     /// @todo make default schemas only once
     JsonPropsWriterFactory factory{};
     const DataSchema schema = make_default_schema( type, factory );
     JsonPropsWriter writer{};
     copy_properties( schema, reader, writer );
     nlohmann::json result{};
     result[ "props" ] = writer.get_result();
     result[ "type" ] = type;
     return result;
}


template < typename T >
nlohmann::json convert_nodes( T& reader )
{
     nlohmann::json piece = nlohmann::json::array();
     if ( reader.is_empty() )
     {
          return piece;
     }

     do
     {
          const auto type = reader.get_current_node_type();
          auto& node_reader = reader.get_current_node_reader();
          piece.push_back( convert_node( type, node_reader ) );
     }
     while ( reader.next_node() );
     return piece;
}


nlohmann::json convert_state( ISceneStateReader& reader )
{
     nlohmann::json result;
     result[ "rendering" ] = reader.get_rendering();
     result[ "updating" ] = reader.get_updating();
     const auto render_system_type = reader.get_render_system_type();

     JsonPropsWriterFactory factory{};
     const DataSchema render_system_schema = make_render_system_schema( render_system_type, factory );
     JsonPropsWriter writer{};
     copy_properties( render_system_schema, reader.get_render_system_reader(), writer );
     nlohmann::json render_system{};
     render_system[ "props" ] = writer.get_result();
     render_system[ "type" ] = render_system_type;
     result[ "render_system" ] = render_system;

     result[ "nodes" ] = convert_nodes( reader );
     return result;
}

} // anonymous namespace


void JsonSceneConvertor::convert_scene( ISceneReader& scene_reader, const std::filesystem::path& out_file )
{
     nlohmann::json scene;
     scene[ "version" ] = NARENGINE_VERSION_UINT32;

     ResourceIndex res_index{};
     std::array< std::uint16_t, 2 > resource{};
     auto res_index_opt = scene_reader.get_setup_func();
     if ( res_index_opt.has_value() )
     {
          res_index = *res_index_opt;
          resource = { res_index.package, res_index.resource };
          scene[ "setup_func" ] = resource;
     }

     res_index_opt = scene_reader.get_loop_func();
     if ( res_index_opt.has_value() )
     {
          res_index = *res_index_opt;
          resource = { res_index.package, res_index.resource };
          scene[ "loop_func" ] = resource;
     }

     const auto deps = scene_reader.get_dependencies();
     store_dependencies( deps, scene );

     const auto piece_count = scene_reader.get_scene_piece_count();
     if ( piece_count > 0 )
     {
          nlohmann::json internal_pieces = nlohmann::json::array();
          for ( std::size_t i = 0; i < piece_count; i++ )
          {
               internal_pieces.push_back(
                    convert_nodes( scene_reader.get_scene_piece_reader( i ) ) );
          }
          scene[ "internal_pieces" ] = internal_pieces;
     }

     nlohmann::json states = nlohmann::json::array();
     if ( !scene_reader.is_empty() )
     {
          do
          {
               auto& state_reader = scene_reader.get_current_state_reader();
               states.push_back( convert_state( state_reader ) );
          }
          while( scene_reader.next_state() );
     }
     scene[ "states" ] = states;

     std::ofstream{ out_file.string() } << scene;
}


void JsonSceneConvertor::convert_node_package( INodePackageReader& reader, const std::filesystem::path& out_file )
{
     nlohmann::json node_package;
     node_package[ "version" ] = NARENGINE_VERSION_UINT32;
     const auto deps = reader.get_dependencies();
     store_dependencies( deps, node_package );

     const auto piece_names = reader.get_scene_piece_names();
     nlohmann::json pieces = nlohmann::json::array();
     for ( const auto& name : piece_names )
     {
          pieces[ name ] =
               convert_nodes( reader.get_scene_piece_reader( name ) );
     }
     node_package[ "pieces" ] = pieces;
     std::ofstream{ out_file.string() } << node_package;
}

} // namespace _16nar::tools::constructor2d
