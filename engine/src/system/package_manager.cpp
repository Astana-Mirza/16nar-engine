#include <16nar/system/package_manager.h>

#include <16nar/render/irender_api.h>
#include <16nar/logger/logger.h>
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
     struct LoadParams< ResourceType::Texture > api_params;

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
          static_cast< std::size_t >(resource->params_type()) + 1);
     std::vector< DataSharedPtr > data;
     std::string name = resource->name()->c_str();
     data.reserve( resource->data_sizes()->size() );

     for ( uint32_t data_size : *resource->data_sizes() )
     {
          data.emplace_back( DataSharedPtr{ new char[ data_size ], std::default_delete< char[] >() } );
          ifs.read( data.back().get(), data_size );
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


PackageManager::PackageManager( IRenderApi& render_api ):
     packages_{}, pkg_dir_{}, render_api_{ render_api }
{}


bool PackageManager::load_package( std::string_view filename, bool unpacked )
{
     LOG_16NAR_DEBUG( "Loading package " << filename );
     if ( is_loaded( filename ) )
     {
          LOG_16NAR_DEBUG( "Package " << filename << " is already loaded" );
          return true;
     }

     if ( unpacked )
     {
          return load_unpacked( filename );
     }

     std::string path = ( pkg_dir_.empty() ? std::string{} : pkg_dir_ + "/" ) + std::string{ filename } + pkgExt;
     std::ifstream ifs{ path, std::ios_base::in | std::ios_base::binary };
     if ( !ifs.is_open() )
     {
          LOG_16NAR_ERROR( "Cannot open package " << filename );
          return false;
     }

     uint32_t header_size = 0;
     ifs.read( reinterpret_cast< char * >( &header_size ), sizeof( header_size ) );
     std::vector< uint8_t > header_buffer( header_size );
     ifs.read( reinterpret_cast< char * >( header_buffer.data() ), header_size );

     if ( !pkg::VerifyPackageBuffer( fb::Verifier( header_buffer.data(), header_size ) ) )
     {
          LOG_16NAR_ERROR( "Package " << filename << " has broken header" );
          return false;
     }

     auto header = pkg::GetPackage( header_buffer.data() );
     auto resources = header->resources();

     bool ok = true;
     std::unordered_map< std::string, Resource > loaded;
     for ( auto iter : *resources )
     {
          auto res = load_resource( iter, render_api_, ifs );
          if ( res.second.type == ResourceType::Unknown )
          {
               ok = false;
               break;
          }
          if ( loaded.find( res.first ) != loaded.cend() )
          {
               LOG_16NAR_ERROR( "Asset " << res.first << " from package "
                    << filename << " has duplicate name" );
               unload_resource( res.second, render_api_ );
               ok = false;
               break;
          }
          loaded.emplace( res );
     }
     if ( ok )
     {
          packages_.emplace( std::string{ filename }, loaded );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package " << filename );
          for ( const auto& res : loaded )
          {
               unload_resource( res.second, render_api_ );
          }
     }
     return ok;
}


void PackageManager::unload_package( std::string_view filename )
{
     LOG_16NAR_DEBUG( "Unloading package " << filename );
     auto iter = packages_.find( std::string{ filename } );
     if ( iter == packages_.cend() )
     {
          return;
     }
     for ( const auto& res : iter->second )
     {
          unload_resource( res.second, render_api_ );
     }
     packages_.erase( iter );
}


bool PackageManager::is_loaded( std::string_view package ) const
{
     return packages_.find( std::string{ package } ) != packages_.cend();
}


bool PackageManager::is_resource_loaded( std::string_view package, std::string_view resource ) const
{
     auto iter = packages_.find( std::string{ package } );
     return iter == packages_.cend()
          && iter->second.find( std::string{ resource } ) != iter->second.cend();
}


void PackageManager::set_package_dir( std::string_view dirname )
{
     pkg_dir_ = dirname;
}


Resource PackageManager::get_resource( std::string_view package, std::string_view resource ) const
{
     auto iter = packages_.find( std::string{ package } );
     if ( iter == packages_.cend() )
     {
          LOG_16NAR_ERROR( "Package '" << package << "' is not loaded" );
          return Resource{};
     }
     auto pkg_iter = iter->second.find( std::string{ resource } );
     if ( pkg_iter == iter->second.cend() )
     {
          LOG_16NAR_ERROR( "No resource '" << resource << "' in package '" << package << "'" );
          return Resource{};
     }
     return pkg_iter->second;
}


bool PackageManager::load_unpacked( std::string_view dirname )
{
     bool ok = true;
     std::unordered_map< std::string, Resource > loaded;
     std::string path = ( pkg_dir_.empty() ? std::string{} : pkg_dir_ + "/" ) + std::string{ dirname };
     for ( const auto& dir_entry : std::filesystem::directory_iterator{ path } )
     {
          if ( !dir_entry.is_regular_file() || dir_entry.path().extension() != assetExt )
          {
               continue;
          }

          std::ifstream ifs{ dir_entry.path(), std::ios_base::in | std::ios_base::binary};
          if ( !ifs.is_open() )
          {
               LOG_16NAR_ERROR( "Cannot open asset " << dir_entry.path() << " during loading package " << dirname );
               ok = false;
               break;
          }

          uint32_t header_size = 0;
          ifs.read( reinterpret_cast< char * >( &header_size ), sizeof( header_size ) );
          std::vector< uint8_t > header_buffer( header_size );
          ifs.read( reinterpret_cast< char * >( header_buffer.data() ), header_size );

          auto resource = pkg::GetResource( header_buffer.data() );

          if ( !pkg::VerifyResourceBuffer( fb::Verifier( header_buffer.data(), header_size ) ) )
          {
               LOG_16NAR_ERROR( "Asset " << dir_entry.path() << " from package " << dirname << " is broken" );
               ok = false;
               break;
          }

          auto res = load_resource( resource, render_api_, ifs );
          if ( res.second.type == ResourceType::Unknown )
          {
               LOG_16NAR_ERROR( "Cannot load asset " << dir_entry.path() << " from package " << dirname );
               ok = false;
               break;
          }
          if ( loaded.find( res.first ) != loaded.cend() )
          {
               LOG_16NAR_ERROR( "Asset " << dir_entry.path() << " from package "
                    << dirname << " has duplicate name" );
               unload_resource( res.second, render_api_ );
               ok = false;
               break;
          }
          loaded.emplace( res );
     }
     if ( ok )
     {
          packages_.emplace( std::string{ dirname }, loaded );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package " << dirname);
          for ( const auto& res : loaded )
          {
               unload_resource( res.second, render_api_ );
          }
     }
     return ok;
}

} // namespace _16nar
