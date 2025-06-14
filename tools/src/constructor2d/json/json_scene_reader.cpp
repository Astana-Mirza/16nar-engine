#include <16nar/tools/constructor2d/json/json_scene_reader.h>

#include <16nar/tools/constructor2d/json/json_read_utils.h>

#include <array>
#include <stdexcept>
#include <cassert>

namespace _16nar::tools::constructor2d
{

JsonSceneReader::JsonSceneReader( const nlohmann::json& json ):
     json_( json ), pieces_{}, state_reader_{},
     end_{}, curr_state_{}
{
     read_json();

     const auto& states = json_.at( "states" );
     curr_state_ = states.cbegin();
     end_ = states.cend();
     if ( curr_state_ != end_ )
     {
          state_reader_ = JsonSceneStateReader{ *curr_state_ };
     }
}


JsonSceneReader::JsonSceneReader( nlohmann::json&& json ):
     json_( std::move( json ) ), pieces_{}, state_reader_{},
     end_{}, curr_state_{}
{
     read_json();

     const auto& states = json_.at( "states" );
     curr_state_ = states.cbegin();
     end_ = states.cend();
     if ( curr_state_ != end_ )
     {
          state_reader_ = JsonSceneStateReader{ *curr_state_ };
     }
}


Dependencies JsonSceneReader::get_dependencies()
{
     return read_scene_deps( json_.at( "deps" ) );
}


ResourceIndex JsonSceneReader::get_setup_func()
{
     return read_resource_index( "setup_func", true );
}


ResourceIndex JsonSceneReader::get_loop_func()
{
     return read_resource_index( "loop_func", true );
}


ResourceIndex JsonSceneReader::get_schema()
{
     return read_resource_index( "schema", false );
}


ISceneStateReader& JsonSceneReader::get_current_state_reader()
{
     return state_reader_;
}


bool JsonSceneReader::next_state()
{
     if ( curr_state_ == end_ )
     {
          return false;
     }
     if ( ++curr_state_ == end_ )
     {
          return false;
     }
     state_reader_ = JsonSceneStateReader{ *curr_state_ };
     return true;
}


IScenePieceReader& JsonSceneReader::get_scene_piece_reader( std::size_t idx )
{
     return pieces_[ idx ];
}


std::size_t JsonSceneReader::get_scene_piece_count()
{
     return pieces_.size();
}


void JsonSceneReader::read_json()
{
     std::uint32_t version = json_.at( "version" );
     assert( ( version & NARENGINE_VERSION_COMPATIBLE_MASK ) == NARENGINE_VERSION_NO_PATCH_UINT32 );

     const auto pieces_iter = json_.find( "internal_pieces" );
     if ( pieces_iter == json_.cend() )
     {
          return; // "internal_pieces" field is optional
     }

     const auto& pieces = *pieces_iter;
     if ( pieces.type() != nlohmann::json::value_t::array
          && pieces.type() != nlohmann::json::value_t::null )
     {
          throw std::runtime_error{ "wrong object type of 'pieces' field" };
     }

     pieces_.reserve( pieces.size() );
     for ( const auto& json : pieces )
     {
          if( json.type() != nlohmann::json::value_t::array
               && json.type() != nlohmann::json::value_t::null )
          {
               throw std::runtime_error{ "wrong object type of a scene piece" };
          }
          pieces_.emplace_back( json );
     }
}


ResourceIndex JsonSceneReader::read_resource_index( std::string_view name, bool optional ) const
{
     const auto iter = json_.find( name );
     if ( iter == json_.cend() )
     {
          if ( !optional )
          {
               throw std::runtime_error{ "mandatory field '" + std::string{ name } + "' is absent" };
          }
          return ResourceIndex{};
     }
     std::array< std::uint16_t, 2 > val = *iter;
     ResourceIndex ret{};
     ret.package = val[ 0 ];
     ret.resource = val[ 1 ];
     return ret;
}

} // namespace _16nar::tools::constructor2d
