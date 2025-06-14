#include <16nar/tools/constructor2d/json/json_read_utils.h>

#include <stdexcept>

namespace _16nar::tools::constructor2d
{

Dependencies read_scene_deps( const nlohmann::json& json )
{
     Dependencies result{};

     const auto& resources = json.at( "resources" );
     if ( resources.type() != nlohmann::json::value_t::array
          && resources.type() != nlohmann::json::value_t::null )
     {
          throw std::runtime_error{ "wrong object type of 'resources' field" };
     }

     result.resources.reserve( resources.size() );
     for ( const auto& res : resources )
     {
          PackageDeps dep{};
          dep.name = res.at( "name" );
          dep.elements = res.at( "elements" );
          result.resources.emplace_back( std::move( dep ) );
     }

     const auto& libs = json.at( "libs" );
     if ( libs.type() != nlohmann::json::value_t::array
          && libs.type() != nlohmann::json::value_t::null )
     {
          throw std::runtime_error{ "wrong object type of 'libs' field" };
     }

     result.libs.reserve( libs.size() );
     for ( const auto& lib : libs )
     {
          PackageDeps dep{};
          dep.name = lib.at( "name" );
          dep.elements = lib.at( "elements" );
          result.libs.emplace_back( std::move( dep ) );
     }

     const auto& pieces = json.at( "pieces" );
     if ( pieces.type() != nlohmann::json::value_t::array
          && pieces.type() != nlohmann::json::value_t::null )
     {
          throw std::runtime_error{ "wrong object type of 'pieces' field" };
     }

     result.pieces.reserve( pieces.size() );
     for ( const auto& piece : pieces )
     {
          PackageDeps dep{};
          dep.name = piece.at( "name" );
          dep.elements = piece.at( "elements" );
          result.pieces.emplace_back( std::move( dep ) );
     }

     return result;
}

} // namespace _16nar::tools::constructor2d
