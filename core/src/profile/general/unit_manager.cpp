#include <16nar/core/profile/general/unit_manager.h>

#include <16nar/core/resources/unified_storage.h>
#include <16nar/core/assets/iasset_file_processor.h>
#include <16nar/core/assets/iasset_reader.h>

#include <16nar/platform/logger/logger.h>

#include <cinttypes>
#include <cassert>

namespace _16nar::profile
{

UnitManager::UnitManager(
     std::pmr::memory_resource& resource, resources::UnifiedStorage& storage ):
     names_( &resource ), info_( &resource ), unload_set_( &resource ),
     resource_{ resource }, storage_{ storage }, next_id_{ 1 }
{}


resources::ResourceHandle UnitManager::load( strings::StaticName name, execution::Mode mode )
{
     const auto iter = names_.find( name );
     if ( iter != names_.cend() )
     {
          const auto info_iter = info_.find( iter->second );
          if ( info_iter == info_.cend() )
          {
               LOG_16NAR_ERROR( "Unit of load manager is inconsistent on "
                    "resource with identifier %" PRIu32 " not found", iter->second );
               return resources::ResourceHandle{};
          }
          auto handle = IResourceManager::create_handle( iter->second );
          if ( load_by_id( iter->second, mode ) )
          {
               ++info_iter->second.ref_count;
               return handle;
          }
          return resources::ResourceHandle{};
     }

     auto handle = preload( name, mode );
     if ( handle && load_by_id( handle.get_id(), mode ) )
     {
          return handle;
     }
     return resources::ResourceHandle{};
}


resources::ResourceHandle UnitManager::preload( strings::StaticName name, execution::Mode )
{
     auto res_id = next_id_;
     const auto pair = names_.emplace( name, res_id );
     if ( !pair.second )
     {
          res_id = pair.first->second;
          const auto iter = info_.find( res_id );
          if ( iter == info_.end() )
          {
               LOG_16NAR_ERROR( "Unit of load manager is inconsistent on "
                    "resource with identifier %" PRIu32 ", data does not exist", res_id );
               return resources::ResourceHandle{};
          }
          switch ( iter->second.state )
          {
               case resources::ResourceState::Preloading:
               {
                    LOG_16NAR_ERROR( "Detected recursive dependency "
                         "on resource with identifier %" PRIu32, res_id );
                    return resources::ResourceHandle{};
               }
               case resources::ResourceState::Unpreloading:
               {
                    unload_set_.erase( res_id );
                    iter->second.state = resources::ResourceState::Preloaded;
                    ++iter->second.ref_count;
                    return IResourceManager::create_handle( res_id );
               }
               case resources::ResourceState::Unloading:
               {
                    unload_set_.erase( res_id );
                    iter->second.state = resources::ResourceState::Loaded;
                    ++iter->second.ref_count;
                    return IResourceManager::create_handle( res_id );
               }
               case resources::ResourceState::Preloaded:    [[ fallthrough ]];
               case resources::ResourceState::Loading:      [[ fallthrough ]];
               case resources::ResourceState::Loaded:
               {
                    ++iter->second.ref_count;
                    return IResourceManager::create_handle( res_id );
               }
               default:
               {
                    LOG_16NAR_ERROR( "Cannot preload unit of load with identifier %"
                         PRIu32 ", unexpected state %hhu ", res_id,
                         static_cast< std::uint8_t >( iter->second.state ) );
                    break;
               }
          }
          return resources::ResourceHandle{};
     }

     const auto data_pair = info_.try_emplace( res_id, resource_ );
     if ( !data_pair.second )
     {
          LOG_16NAR_ERROR( "Unit of load manager is inconsistent on "
               "resource with identifier %" PRIu32 ", data already exists", res_id );
          return resources::ResourceHandle{};
     }
     ++next_id_;

     auto& info = data_pair.first->second;
     info.name = name;
     info.ref_count = 1;
     info.state = resources::ResourceState::Preloading;
     preload_task( info );
     return IResourceManager::create_handle( res_id );
}


bool UnitManager::load_by_id( resources::ResID res_id, execution::Mode )
{
     const auto iter = info_.find( res_id );
     if ( iter == info_.end() )
     {
          LOG_16NAR_ERROR( "Cannot load unit of load, identifier %" PRIu32 " not found", res_id );
          return false;
     }
     UnitInfo& info = iter->second;

     switch ( info.state )
     {
          case resources::ResourceState::Unpreloading:
          {
               unload_set_.erase( res_id );
               info.state = resources::ResourceState::Preloaded;
               [[ fallthrough ]];
          }
          case resources::ResourceState::Preloaded:
          {
               info.state = resources::ResourceState::Loading;
               load_task( info );
               return true;
          }
          case resources::ResourceState::Unloading:
          {
               unload_set_.erase( res_id );
               info.state = resources::ResourceState::Loaded;
               [[ fallthrough ]];
          }
          case resources::ResourceState::Loading: [[ fallthrough ]];
          case resources::ResourceState::Loaded:
          {
               return true;
          }
          default:
          {
               LOG_16NAR_ERROR( "Cannot load unit of load with identifier %"
                    PRIu32 ", unexpected state %hhu ", res_id,
                    static_cast< std::uint8_t >( info.state ) );
               break;
          }
     }
     return false;
}


void UnitManager::instantiate( resources::ResID )
{
     /// @todo check that the UoL is at least preloaded AND not unloading/unpreloading
}


bool UnitManager::wait_preload( resources::ResID )
{
     /// @todo wait for preload task to complete
     return true;
}


bool UnitManager::wait_load( resources::ResID )
{
     /// @todo wait for load task to complete
     return true;
}


void UnitManager::inc_ref( resources::ResID res_id )
{
     const auto iter = info_.find( res_id );
     if ( iter == info_.cend() )
     {
          LOG_16NAR_ERROR( "Cannot increment reference of unit of load, identifier %" PRIu32 " not found", res_id );
          return;
     }
     assert( iter->second.ref_count > 0 );
     assert( iter->second.state == resources::ResourceState::Loaded
          || iter->second.state == resources::ResourceState::Loading
          || iter->second.state == resources::ResourceState::Preloaded );
     ++iter->second.ref_count;
}


void UnitManager::dec_ref( resources::ResID res_id )
{
     const auto iter = info_.find( res_id );
     if ( iter == info_.cend() )
     {
          LOG_16NAR_ERROR( "Cannot decrement reference of unit of load, identifier %" PRIu32 " not found", res_id );
          return;
     }
     assert( iter->second.ref_count > 0 );
     if ( --iter->second.ref_count > 0 )
     {
          return;
     }

     switch ( iter->second.state )
     {
          case resources::ResourceState::Preloaded:
          {
               iter->second.state = resources::ResourceState::Unpreloading;
               break;
          }
          case resources::ResourceState::Loading: [[ fallthrough ]];
          case resources::ResourceState::Loaded:
          {
               iter->second.state = resources::ResourceState::Unloading;
               break;
          }
          default:
          {
               LOG_16NAR_WARNING( "Reference count of unit of load with identifier % "
                    PRIu32 " became 0 in unexpected state  %hhu",
                    res_id, static_cast< std::uint8_t >( iter->second.state ) );
               break;
          }
     }
     unload_set_.insert( res_id );
}


void UnitManager::unload_delayed()
{
     /// @todo traverse unload_set_ and unload it all
}


void UnitManager::preload_task( UnitManager::UnitInfo& info )
{
     // need unified storage + all managers
     /// @todo preload process: fill "data" and "deps" fields

     info.data = storage_.load_asset( info.name );
     if ( !info.data )
     {
          LOG_16NAR_ERROR( "Cannot preload unit of load" );
          info.state = resources::ResourceState::Unloaded;
          return;
     }

     info.reader = storage_.get_file_processor()->make_asset_reader();
     info.reader->reset( info.data.get_const_view() );

     info.state = resources::ResourceState::Preloaded;
}


void UnitManager::load_task( UnitManager::UnitInfo& info )
{
     for ( auto& handle : info.deps )
     {
          handle.load( execution::Mode::Async );
     }
     for ( auto& handle : info.deps )
     {
          handle.wait_load();
     }
     info.state = resources::ResourceState::Loaded;
}


UnitManager::UnitInfo::UnitInfo( std::pmr::memory_resource& resource ):
     deps( &resource ), data{}, reader{}, name{},
     ref_count{}, state{ resources::ResourceState::Unloaded }
{}

} // namespace _16nar::profile
