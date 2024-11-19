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

PackageManager::PackageManager( std::unique_ptr< tools::IAssetReader >&& reader ):
     resources_{}, names_{}, packages_{}, pkg_dir_{},
     reader_{ std::move( reader ) }, unpacked_mode_{ false }
{}


PackageManager::~PackageManager()
{
     clear();
}


bool PackageManager::load_package( std::string_view name ) noexcept
{
     if ( is_package_loaded( name ) )
     {
          LOG_16NAR_DEBUG( "Package '" << name << "' is already loaded" );
          return true;
     }
     if ( unpacked_mode_ )
     {
          return load_unpacked( name );
     }

     LOG_16NAR_DEBUG( "Loading package '" << name << "'..." );
     std::string pkg_name{ name };
     std::string path{ name };
     if ( !pkg_dir_.empty() )
     {
          path = ( std::filesystem::path{ pkg_dir_ } / ( pkg_name + "." + reader_->get_pkg_ext() ) ).string();
     }
     std::ifstream ifs{ path, std::ios_base::in | std::ios_base::binary };
     if ( !ifs.is_open() )
     {
          LOG_16NAR_ERROR( "Cannot open package '" << path << "'" );
          return false;
     }
     bool ok = true;
     ResourceMap loaded;
     NameMap loaded_names;
     tools::PackageData pkg{};
     try
     {
          pkg = reader_->read_package( ifs );
     }
     catch ( const std::exception& ex )
     {
          LOG_16NAR_ERROR( "Error loading package '" << pkg_name
               << "': " << ex.what() );
          return false;
     }
     auto& render_api = get_game().get_render_api();
     for ( const auto& load_data : pkg.resources )
     {
          if ( loaded.find( load_data.name ) != loaded.cend() )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << pkg_name << "' has duplicate name" );
               ok = false;
               break;
          }
          Resource resource{};
          try
          {
               resource = render_api.load( load_data.type, load_data.params );
          }
          catch ( const std::exception& ex )
          {
               LOG_16NAR_ERROR( "Error loading asset '" << load_data.name << "' from package '"
                    << pkg_name << "': " << ex.what() );
               ok = false;
               break;
          }
          std::string full_name = pkg_name + "/" + load_data.name;
          auto [ new_iter, insert_ok ] = loaded.emplace( full_name, resource );
          if ( !insert_ok )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << pkg_name << "' cannot be saved" );
               render_api.unload( resource );
               ok = false;
               break;
          }
          if ( !loaded_names.emplace( resource, new_iter ).second )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << pkg_name << "' cannot be saved" );
               // unload will happen later because ok is false
               ok = false;
               break;
          }
     }
     if ( ok && !packages_.emplace( pkg_name ).second )
     {
          LOG_16NAR_ERROR( "Package '" << pkg_name << "' cannot be saved" );
          ok = false;
     }
     if ( ok )
     {
          resources_.merge( loaded );
          names_.merge( loaded_names );
          LOG_16NAR_INFO( "Package '" << pkg_name << "' loaded successfully" );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package '" << pkg_name << "'" );
          for ( const auto& res : loaded )
          {
               render_api.unload( res.second );
          }
     }
     return ok;
}


void PackageManager::unload_package( std::string_view name ) noexcept
{
     LOG_16NAR_DEBUG( "Unloading package '" << name << "'..." );
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
               render_api.unload( iter->second );
               names_.erase( iter->second );
               iter = resources_.erase( iter );
          }
          else
          {
               ++iter;
          }
     }
     packages_.erase( pkg_iter );
     LOG_16NAR_INFO( "Package '" << name << "' unloaded successfully" );
}


bool PackageManager::is_package_loaded( std::string_view name ) const noexcept
{
     return packages_.find( std::string{ name } ) != packages_.cend();
}


bool PackageManager::is_resource_loaded( std::string_view name ) const noexcept
{
     return resources_.find( std::string{ name } ) != resources_.cend();
}


void PackageManager::set_unpacked_mode( bool mode ) noexcept
{
     unpacked_mode_ = mode;
}


void PackageManager::set_package_dir( std::string_view dirname ) noexcept
{
     pkg_dir_ = dirname;
}


Resource PackageManager::get_resource( std::string_view name ) const noexcept
{
     auto iter = resources_.find( std::string{ name } );
     if ( iter == resources_.cend() )
     {
          LOG_16NAR_ERROR( "No such resource '" << name << "'" );
          return Resource{};
     }
     return iter->second;
}


void PackageManager::clear() noexcept
{
     if ( resources_.empty() && names_.empty()
          && packages_.empty() && pkg_dir_.empty() )
     {
          return;
     }
     LOG_16NAR_DEBUG( "Unloading all resource packages..." );
     auto& render_api = get_game().get_render_api();
     auto iter = resources_.begin();
     while ( iter != resources_.end() )
     {
          render_api.unload( iter->second );
          iter = resources_.erase( iter );
     }
     names_.clear();
     packages_.clear();
     pkg_dir_.clear();
     LOG_16NAR_INFO( "All resource packages unloaded successfully" );
}


bool PackageManager::load_unpacked( std::string_view dirname ) noexcept
{
     LOG_16NAR_DEBUG( "Loading package '" << dirname << "' (unpacked)..." );
     bool ok = true;
     std::string pkg_name{ dirname };
     ResourceMap loaded;
     NameMap loaded_names;
     auto& render_api = get_game().get_render_api();
     std::string path{ dirname };
     if ( !pkg_dir_.empty() )
     {
          path = ( std::filesystem::path{ pkg_dir_ } / pkg_name ).string();
     }
     for ( const auto& dir_entry : std::filesystem::directory_iterator{ path } )
     {
          if ( !dir_entry.is_regular_file() || dir_entry.path().extension() != reader_->get_file_ext() )
          {
               continue;
          }

          std::ifstream ifs{ dir_entry.path(), std::ios_base::in | std::ios_base::binary };
          if ( !ifs.is_open() )
          {
               LOG_16NAR_ERROR( "Cannot open asset '" << dir_entry.path()
                    << "' during loading package '" << pkg_name << "'" );
               ok = false;
               break;
          }

          tools::ResourceData load_data{};
          Resource resource{};
          try
          {
               load_data = reader_->read_asset( ifs );
               if ( loaded.find( load_data.name ) != loaded.cend() )
               {
                    LOG_16NAR_ERROR( "Asset '" << dir_entry.path() << "' from package '"
                         << pkg_name << "' has duplicate name" );
                    ok = false;
                    break;
               }
               resource = render_api.load( load_data.type, load_data.params );
          }
          catch ( const std::exception& ex )
          {
               LOG_16NAR_ERROR( "Cannot load asset '" << dir_entry.path()
                    << "' from package '" << pkg_name << "': " << ex.what() );
               ok = false;
               break;
          }
          std::string full_path = pkg_name + '/' + load_data.name;
          auto [ new_iter, insert_ok ] = loaded.emplace( full_path, resource );
          if ( !insert_ok )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << pkg_name << "' cannot be saved" );
               render_api.unload( resource );
               ok = false;
               break;
          }
          if ( !loaded_names.emplace( resource, new_iter ).second )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << pkg_name << "' cannot be saved" );
               // unload will happen later because ok is false
               ok = false;
               break;
          }
     }
     if ( ok && !packages_.emplace( pkg_name ).second )
     {
          LOG_16NAR_ERROR( "Package '" << pkg_name << "' cannot be saved" );
          ok = false;
     }
     if ( ok )
     {
          resources_.merge( loaded );
          names_.merge( loaded_names );
          LOG_16NAR_INFO( "Package '" << pkg_name << "' (unpacked) loaded successfully" );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package '" << pkg_name << "'" );
          for ( const auto& res : loaded )
          {
               render_api.unload( res.second );
          }
     }
     return ok;
}

} // namespace _16nar
