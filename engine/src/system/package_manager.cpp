#include <16nar/system/package_manager.h>

#include <16nar/render/irender_api.h>
#include <16nar/logger/logger.h>
#include <16nar/game.h>

#include <16nar/gen/package_generated.h>
#include <16nar/gen/resource_generated.h>

#include <fstream>
#include <vector>
#include <filesystem>

namespace _16nar
{

namespace pkg = data::package;
namespace fb = ::flatbuffers;

namespace
{

constexpr char pkgExt[] = ".nrs";
constexpr char assetExt[] = ".narasset";


Resource load_texture( const pkg::TextureLoadParams* params,
     const std::vector< DataSharedPtr >& data, IRenderApi& api )
{
     LoadParams< ResourceType::Texture > api_params;

     //api_params.

     return Resource{};
}


Resource load_shader( const pkg::ShaderLoadParams* params,
     const std::vector< DataSharedPtr >& data, IRenderApi& api )
{
     return Resource{};
}


Resource load_vertex_buffer( const pkg::VertexBufferLoadParams* params,
     const std::vector< DataSharedPtr >& data, IRenderApi& api )
{
     return Resource{};
}


Resource load_cubemap( const pkg::CubemapLoadParams* params,
     const std::vector< DataSharedPtr >& data, IRenderApi& api )
{
     return Resource{};
}


std::pair< std::string, Resource > load_resource( const pkg::Resource* resource, IRenderApi& api, std::ifstream& ifs )
{
     ResourceType type = static_cast< ResourceType >(
          static_cast< std::size_t >( resource->params_type() ) + 1 );
     std::vector< DataSharedPtr > data;
     std::string name = resource->name()->c_str();
     data.reserve( resource->data_sizes()->size() );

     for ( uint32_t data_size : *resource->data_sizes() )
     {
          data.emplace_back( DataSharedPtr{ new std::byte[ data_size ], std::default_delete< std::byte[] >() } );
          ifs.read( reinterpret_cast< char * >( data.back().get() ), data_size );
     }

     switch ( type )
     {
          case ResourceType::Texture:
          {
               auto params = resource->params_as_TextureLoadParams();
               return std::make_pair( name, load_texture( params, data, api ) );
          }
          case ResourceType::Shader:
          {
               auto params = resource->params_as_ShaderLoadParams();
               return std::make_pair( name, load_shader( params, data, api ) );
          }
          case ResourceType::VertexBuffer:
          {
               auto params = resource->params_as_VertexBufferLoadParams();
               return std::make_pair( name, load_vertex_buffer( params, data, api ) );
          }
          case ResourceType::Cubemap:
          {
               auto params = resource->params_as_CubemapLoadParams();
               return std::make_pair( name, load_cubemap( params, data, api ) );
          }
          default:
          {
               LOG_16NAR_ERROR( "Wrong '" << name << "' type in package: " << static_cast< std::size_t >( type ) );
               return std::make_pair( name, Resource{} );
          }
     }
}


void unload_resource( const Resource& resource, IRenderApi& api )
{
     switch ( resource.type )
     {
          case ResourceType::Texture:
          case ResourceType::Shader:
          case ResourceType::VertexBuffer:
          case ResourceType::Cubemap:
          {
               api.unload( resource );
          }
          break;
          default:
          {
               LOG_16NAR_ERROR( "Wrong resource type: " << static_cast< std::size_t >( resource.type ) );
          }
     }
}

} // anonymous namespace


bool PackageManager::load_package( std::string_view name, bool unpacked )
{
     LOG_16NAR_DEBUG( "Loading package " << name );
     if ( is_package_loaded( name ) )
     {
          LOG_16NAR_DEBUG( "Package " << name << " is already loaded" );
          return true;
     }

     if ( unpacked )
     {
          return load_unpacked( name );
     }

     std::string pkg_name{ name };
     std::string path = ( pkg_dir_.empty() ? std::string{} : pkg_dir_ + "/" ) + pkg_name + pkgExt;
     std::ifstream ifs{ path, std::ios_base::in | std::ios_base::binary };
     if ( !ifs.is_open() )
     {
          LOG_16NAR_ERROR( "Cannot open package " << pkg_name );
          return false;
     }

     uint32_t header_size = 0;
     ifs.read( reinterpret_cast< char * >( &header_size ), sizeof( header_size ) );
     std::vector< uint8_t > header_buffer( header_size );
     ifs.read( reinterpret_cast< char * >( header_buffer.data() ), header_size );

     fb::Verifier verifier{ header_buffer.data(), header_size };
     if ( !pkg::VerifyPackageBuffer( verifier ) )
     {
          LOG_16NAR_ERROR( "Package " << pkg_name << " has broken header" );
          return false;
     }

     auto header = pkg::GetPackage( header_buffer.data() );
     auto resources = header->resources();

     bool ok = true;
     auto& render_api = get_game().get_render_api();
     ResourceMap loaded;
     NameMap loaded_names;
     for ( auto iter : *resources )
     {
          auto res = load_resource( iter, render_api, ifs );
          if ( res.second.type == ResourceType::Unknown )
          {
               ok = false;
               break;
          }
          res.first = pkg_name + "/" + res.first;
          if ( loaded.find( res.first ) != loaded.cend() )
          {
               LOG_16NAR_ERROR( "Asset " << res.first << " from package "
                    << pkg_name << " has duplicate name" );
               unload_resource( res.second, render_api );
               ok = false;
               break;
          }
          auto [ new_iter, insert_ok ] = loaded.emplace( res );
          if ( !insert_ok )
          {
               LOG_16NAR_ERROR( "Asset " << res.first << " from package "
                    << pkg_name << " cannot be saved" );
               unload_resource( res.second, render_api );
               ok = false;
               break;
          }
          loaded_names.emplace( res.second, new_iter );
     }
     if ( ok )
     {
          resources_.merge( loaded );
          names_.merge( loaded_names );
          packages_.emplace( pkg_name );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package " << pkg_name );
          for ( const auto& res : loaded )
          {
               unload_resource( res.second, render_api );
          }
     }
     return ok;
}


void PackageManager::unload_package( std::string_view name )
{
     LOG_16NAR_DEBUG( "Unloading package " << name );
     auto pkg_iter = packages_.find( std::string{ name } );
     if ( pkg_iter == packages_.cend() )
     {
          return;
     }
     std::string prefix = std::string{ name } + '/';
     auto& render_api = get_game().get_render_api();
     for ( auto iter = resources_.cbegin(); iter != resources_.cend(); /* skip ++iter due to erase */ )
     {
          if ( iter->first.find( prefix ) == 0 )
          {
               unload_resource( iter->second, render_api );
               names_.erase( iter->second );
               iter = resources_.erase( iter );
          }
          else
          {
               ++iter;
          }
     }
     packages_.erase( pkg_iter );
}


bool PackageManager::is_package_loaded( std::string_view name ) const
{
     return packages_.find( std::string{ name } ) != packages_.cend();
}


bool PackageManager::is_resource_loaded( std::string_view name ) const
{
     return resources_.find( std::string{ name } ) != resources_.cend();
}


void PackageManager::set_package_dir( std::string_view dirname )
{
     pkg_dir_ = dirname;
}


Resource PackageManager::get_resource( std::string_view name ) const
{
     auto iter = resources_.find( std::string{ name } );
     if ( iter == resources_.cend() )
     {
          LOG_16NAR_ERROR( "No such resource '" << name << "'" );
          return Resource{};
     }
     return iter->second;
}


bool PackageManager::load_unpacked( std::string_view dirname )
{
     bool ok = true;
     std::string pkg_name{ dirname };
     ResourceMap loaded;
     NameMap loaded_names;
     auto& render_api = get_game().get_render_api();
     std::string path = ( pkg_dir_.empty() ? std::string{} : pkg_dir_ + "/" ) + pkg_name;
     for ( const auto& dir_entry : std::filesystem::directory_iterator{ path } )
     {
          if ( !dir_entry.is_regular_file() || dir_entry.path().extension() != assetExt )
          {
               continue;
          }

          std::ifstream ifs{ dir_entry.path(), std::ios_base::in | std::ios_base::binary };
          if ( !ifs.is_open() )
          {
               LOG_16NAR_ERROR( "Cannot open asset " << dir_entry.path() << " during loading package " << pkg_name );
               ok = false;
               break;
          }

          uint32_t header_size = 0;
          ifs.read( reinterpret_cast< char * >( &header_size ), sizeof( header_size ) );
          std::vector< uint8_t > header_buffer( header_size );
          ifs.read( reinterpret_cast< char * >( header_buffer.data() ), header_size );

          auto resource = pkg::GetResource( header_buffer.data() );

          fb::Verifier verifier{ header_buffer.data(), header_size };
          if ( !pkg::VerifyResourceBuffer( verifier ) )
          {
               LOG_16NAR_ERROR( "Asset " << dir_entry.path() << " from package " << pkg_name << " is broken" );
               ok = false;
               break;
          }

          auto res = load_resource( resource, render_api, ifs );
          if ( res.second.type == ResourceType::Unknown )
          {
               LOG_16NAR_ERROR( "Cannot load asset " << dir_entry.path() << " from package " << pkg_name );
               ok = false;
               break;
          }
          if ( loaded.find( res.first ) != loaded.cend() )
          {
               LOG_16NAR_ERROR( "Asset " << dir_entry.path() << " from package "
                    << pkg_name << " has duplicate name" );
               unload_resource( res.second, render_api );
               ok = false;
               break;
          }
          auto [ new_iter, insert_ok ] = loaded.emplace( res );
          if ( !insert_ok )
          {
               LOG_16NAR_ERROR( "Asset " << res.first << " from package "
                    << pkg_name << " cannot be saved" );
               unload_resource( res.second, render_api );
               ok = false;
               break;
          }
          loaded_names.emplace( res.second, new_iter );
     }
     if ( ok )
     {
          resources_.merge( loaded );
          names_.merge( loaded_names );
          packages_.emplace( pkg_name );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package " << pkg_name );
          for ( const auto& res : loaded )
          {
               unload_resource( res.second, render_api );
          }
     }
     return ok;
}

} // namespace _16nar
