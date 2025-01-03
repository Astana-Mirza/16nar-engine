#include <16nar/system/package_manager.h>

#include <16nar/render/irender_api.h>
#include <16nar/logger/logger.h>
#include <16nar/system/exceptions.h>
#include <16nar/game.h>

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


bool PackageManager::load_package( const std::string& name )
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
     std::string path{ name };
     if ( !pkg_dir_.empty() )
     {
          path = ( std::filesystem::path{ pkg_dir_ } / ( name + "." + reader_->get_pkg_ext() ) ).string();
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
          LOG_16NAR_ERROR( "Error loading package '" << name
               << "': " << ex.what() );
          return false;
     }
     auto& render_api = get_game().get_render_api();
     for ( const auto& load_data : pkg.resources )
     {
          if ( loaded.find( load_data.name ) != loaded.cend() )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << name << "' has duplicate name" );
               ok = false;
               break;
          }
          Resource resource{};
          try
          {
               resource = render_api.load( load_data.type, load_data.params );
          }
          catch ( const ResourceException& ex )
          {
               LOG_16NAR_ERROR( "Error loading asset '" << load_data.name << "' from package '"
                    << name << "': " << ex.what() );
               ok = false;
               break;
          }
          std::string full_name = name + "/" + load_data.name;
          auto [ new_iter, insert_ok ] = loaded.emplace( full_name, resource );
          if ( !insert_ok )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << name << "' cannot be saved" );
               render_api.unload( resource );
               ok = false;
               break;
          }
          if ( !loaded_names.emplace( resource, new_iter ).second )
          {
               LOG_16NAR_ERROR( "Asset name '" << load_data.name << "' from package '"
                    << name << "' cannot be saved" );
               // unload will happen later because ok is false
               ok = false;
               break;
          }
     }
     if ( ok && !packages_.emplace( name ).second )
     {
          LOG_16NAR_ERROR( "Package '" << name << "' cannot be saved" );
          ok = false;
     }
     if ( ok )
     {
          resources_.merge( loaded );
          names_.merge( loaded_names );
          LOG_16NAR_INFO( "Package '" << name << "' loaded successfully" );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package '" << name << "'" );
          for ( const auto& res : loaded )
          {
               render_api.unload( res.second );
          }
     }
     return ok;
}


void PackageManager::unload_package( const std::string& name )
{
     LOG_16NAR_DEBUG( "Unloading package '" << name << "'..." );
     auto pkg_iter = packages_.find( name );
     if ( pkg_iter == packages_.cend() )
     {
          return;
     }
     std::string prefix = name + '/';
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


bool PackageManager::is_package_loaded( const std::string& name ) const
{
     return packages_.find( name ) != packages_.cend();
}


bool PackageManager::is_resource_loaded( const std::string& name ) const
{
     return resources_.find( name ) != resources_.cend();
}


void PackageManager::set_unpacked_mode( bool mode ) noexcept
{
     unpacked_mode_ = mode;
}


void PackageManager::set_package_dir( const std::string& dirname )
{
     pkg_dir_ = dirname;
}


Resource PackageManager::get_resource( const std::string& name ) const
{
     auto iter = resources_.find( name );
     if ( iter == resources_.cend() )
     {
          LOG_16NAR_ERROR( "No such resource '" << name << "'" );
          return Resource{};
     }
     return iter->second;
}


void PackageManager::clear()
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


bool PackageManager::load_unpacked( const std::string& dirname )
{
     LOG_16NAR_DEBUG( "Loading package '" << dirname << "' (unpacked)..." );
     bool ok = true;
     ResourceMap loaded;
     NameMap loaded_names;
     auto& render_api = get_game().get_render_api();
     std::string path{ dirname };
     if ( !pkg_dir_.empty() )
     {
          path = ( std::filesystem::path{ pkg_dir_ } / dirname ).string();
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
                    << "' during loading package '" << dirname << "'" );
               ok = false;
               break;
          }

          tools::ResourceData load_data{};
          Resource resource{};
          try
          {
               load_data = reader_->read_asset( ifs );
          }
          catch ( const std::exception& ex )
          {
               LOG_16NAR_ERROR( "Cannot load asset '" << dir_entry.path()
                    << "' from package '" << dirname << "': " << ex.what() );
               ok = false;
               break;
          }
          if ( loaded.find( load_data.name ) != loaded.cend() )
          {
               LOG_16NAR_ERROR( "Asset '" << dir_entry.path() << "' from package '"
                    << dirname << "' has duplicate name" );
               ok = false;
               break;
          }
          try
          {
               resource = render_api.load( load_data.type, load_data.params );
          }
          catch ( const ResourceException& ex )
          {
               LOG_16NAR_ERROR( "Error loading asset '" << load_data.name << "' from package '"
                    << dirname << "': " << ex.what() );
               ok = false;
               break;
          }
          std::string full_path = dirname + '/' + load_data.name;
          auto [ new_iter, insert_ok ] = loaded.emplace( full_path, resource );
          if ( !insert_ok )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << dirname << "' cannot be saved" );
               render_api.unload( resource );
               ok = false;
               break;
          }
          if ( !loaded_names.emplace( resource, new_iter ).second )
          {
               LOG_16NAR_ERROR( "Asset '" << load_data.name << "' from package '"
                    << dirname << "' cannot be saved" );
               // unload will happen later because ok is false
               ok = false;
               break;
          }
     }
     if ( ok && !packages_.emplace( dirname ).second )
     {
          LOG_16NAR_ERROR( "Package '" << dirname << "' cannot be saved" );
          ok = false;
     }
     if ( ok )
     {
          resources_.merge( loaded );
          names_.merge( loaded_names );
          LOG_16NAR_INFO( "Package '" << dirname << "' (unpacked) loaded successfully" );
     }
     else
     {
          LOG_16NAR_ERROR( "Cannot load package '" << dirname << "'" );
          for ( const auto& res : loaded )
          {
               render_api.unload( res.second );
          }
     }
     return ok;
}

} // namespace _16nar
