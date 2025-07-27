#include <16nar/tools/constructor2d/json/json_node_package_reader.h>

#include <16nar/tools/constructor2d/json/json_read_utils.h>

#include <cassert>
#include <stdexcept>

namespace _16nar::tools::constructor2d
{

JsonNodePackageReader::JsonNodePackageReader( const nlohmann::json& json ):
     json_( json ), pieces_{}
{
     read_json();
}


JsonNodePackageReader::JsonNodePackageReader( nlohmann::json&& json ):
     json_( std::move( json ) ), pieces_{}
{
     read_json();
}


Dependencies JsonNodePackageReader::get_dependencies()
{
     return read_scene_deps( json_.at( "deps" ) );
}


IScenePieceReader& JsonNodePackageReader::get_scene_piece_reader( const std::string& name )
{
     return pieces_.at( name );
}


std::size_t JsonNodePackageReader::get_scene_piece_count()
{
     return pieces_.size();
}


std::vector< std::string > JsonNodePackageReader::get_scene_piece_names()
{
     std::vector< std::string > names;
     names.reserve( pieces_.size() );
     for ( const auto& iter : pieces_ )
     {
          names.emplace_back( iter.first );
     }
     return names;
}


void JsonNodePackageReader::read_json()
{
     std::uint32_t version = json_.at( "version" );
     assert( ( version & NARENGINE_VERSION_COMPATIBLE_MASK ) == NARENGINE_VERSION_NO_PATCH_UINT32 );

     const auto& pieces = json_.at( "pieces" );
     if ( pieces.type() != nlohmann::json::value_t::object
          && pieces.type() != nlohmann::json::value_t::null )
     {
          throw std::runtime_error{ "wrong object type of 'pieces' field" };
     }

     for ( const auto& [ key, value ] : pieces.items() )
     {
          if ( value.type() != nlohmann::json::value_t::array
               && value.type() != nlohmann::json::value_t::null )
          {
               throw std::runtime_error{ "wrong object type of one of scene pieces" };
          }
          pieces_.emplace( key, JsonScenePieceReader{ value } );
     }
}


} // namespace _16nar::tools::constructor2d
