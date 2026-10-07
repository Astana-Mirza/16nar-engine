/// @file
/// @brief File with UnitManager class definition.
#ifndef _16NAR_CORE_PROFILE_GENERAL_UNIT_MANAGER_H
#define _16NAR_CORE_PROFILE_GENERAL_UNIT_MANAGER_H

#include <16nar/core/defs.h>

#include <16nar/core/resources/iresource_manager.h>
#include <16nar/core/resources/iresource_processor.h>
#include <16nar/core/assets/defs.h>

#include <unordered_map>
#include <unordered_set>

namespace _16nar::profile
{

/// @brief Manager of units of load (UoLs).
/// @details Units of load are treated as resources, so they may be
/// loaded as usual. The manager is used to control the lifetime of
/// the units.
///
/// Units of load can represent predefined objects, whole game levels
/// or parts of them. Note that loading a unit does not mean
/// that it will be immediately instantiated. Instead, instatiation
/// and load are different operations, so unit of load may be
/// instantiated multiple times after load.
class NARENGINE_CORE_API UnitManager : public resources::IResourceManager
{
public:
     /// @brief Constructor.
     /// @param[in] resource memory resource for data allocations.
     /// @param[in] storage unified storage of resources.
     UnitManager( std::pmr::memory_resource& resource, resources::UnifiedStorage& storage );

     /// @copydoc resources::IResourceManager::load(strings::StaticName, execution::Mode)
     resources::ResourceHandle load( strings::StaticName name, execution::Mode mode ) override;

     /// @copydoc resources::IResourceManager::preload(strings::StaticName, execution::Mode)
     resources::ResourceHandle preload( strings::StaticName name, execution::Mode mode ) override;

     /// @brief Instantiate the scene.
     /// @param[in] res_id resource identifier of the UoL.
     void instantiate( resources::ResID res_id );

protected:
     /// @copydoc resources::IResourceManager::load_by_id(resources::ResID, execution::Mode)
     bool load_by_id( resources::ResID res_id, execution::Mode mode ) override;

     /// @copydoc resources::IResourceManager::wait_preload(resources::ResID)
     bool wait_preload( resources::ResID res_id ) override;

     /// @copydoc resources::IResourceManager::wait_load(resources::ResID)
     bool wait_load( resources::ResID res_id ) override;

     /// @copydoc resources::IResourceManager::inc_ref(resources::ResID)
     void inc_ref( resources::ResID res_id ) override;

     /// @copydoc resources::IResourceManager::dec_ref(resources::ResID)
     void dec_ref( resources::ResID res_id ) override;

private:
     UnitManager( const UnitManager& ) = delete;
     UnitManager& operator=( const UnitManager& ) = delete;

     /// @brief Information about the resource.
     struct UnitInfo
     {
          /// @brief Constructor.
          /// @param[in] resource memory resource for data allocations.
          UnitInfo( std::pmr::memory_resource& resource );

          std::pmr::vector< resources::ResourceHandle > deps;    ///< dependencies of the unit needed for load.
          memory::SharedBufferPtr data;                          ///< data of the resource.
          assets::IAssetReaderPtr reader;                        ///< reader of resource's asset data.
          strings::StaticName name;                              ///< name of the resource.
          std::uint32_t ref_count;                               ///< count of references of the unit.
          resources::ResourceState state;                        ///< state of resource lifecycle.
     };

     /// @brief Process unload of resources which have been scheduled for it.
     /// @details This function is called periodically, e.g. in the end of the frame.
     void unload_delayed();

     /// @brief Implementation of preload process of unit of load.
     /// @param info information about the UoL.
     void preload_task( UnitInfo& info );

     /// @brief Implementation of load process of unit of load.
     /// @param info information about the UoL.
     void load_task( UnitInfo& info );

private:
     std::pmr::unordered_map< strings::StaticName, resources::ResID > names_;   ///< resource names.
     std::pmr::unordered_map< resources::ResID, UnitInfo > info_;     ///< resources description.
     std::pmr::unordered_set< resources::ResID > unload_set_;         ///< set of resources to be unloaded.
     std::pmr::memory_resource& resource_;                            ///< memory resource for data allocations.
     resources::UnifiedStorage& storage_;                             ///< unified storage of resources.
     resources::ResID next_id_;                                       ///< next resource identifier.
};

} // namespace _16nar::profile

#endif // #ifndef _16NAR_CORE_PROFILE_GENERAL_UNIT_MANAGER_H
