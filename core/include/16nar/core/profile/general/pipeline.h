/// @file
/// @brief File with Pipeline class definition.
#ifndef _16NAR_CORE_PROFILE_GENERAL_PIPELINE_H
#define _16NAR_CORE_PROFILE_GENERAL_PIPELINE_H

#include <16nar/core/defs.h>

#include <16nar/platform/memory/memory_manager.h>
#include <16nar/platform/strings/defs.h>

#include <16nar/core/assets/defs.h>
#include <16nar/core/resources/defs.h>
#include <16nar/core/ecs/defs.h>
#include <16nar/core/config/defs.h>
#include <16nar/core/loader/service_locator.h>

#include <unordered_map>

namespace _16nar::profile
{

/// @brief Framework class which implements application execution pipeline.
class NARENGINE_CORE_API Pipeline
{
public:
     /// @brief Data for pipeline initialization.
     struct InitData
     {
          assets::IAssetFileProcessorPtr file_processor{};  ///< asset file processor.
     };

     /// @brief Constructor.
     /// @throws std::runtime_error in case of pipeline setup errors.
     /// @param[in] root_domain root memory domain.
     explicit Pipeline( memory::IMemoryDomain& root_domain );

     /// @brief Destructor.
     ~Pipeline();

     /// @brief Initialize the pipeline using custom parameters.
     /// @throws std::runtime_error in case of pipeline initialization errors.
     /// @param[in] data data for pipeline initialization.
     void init( InitData&& data );

     /// @brief Get memory manager.
     /// @note This function may be used even when the pipeline is not initialized.
     /// @return memory manager.
     memory::MemoryManager& get_memory_manager() noexcept;

     /// @brief Check if the pipeline is initialized.
     /// @return true if the pipeline is initialized, false otherwise.
     bool is_initialized() const noexcept;

     /// @brief Get name manager.
     /// @warning The pipeline must be initialized before calling this function.
     /// @return name manager.
     strings::NameManager& get_name_manager() noexcept;

     /// @brief Get resource manager if it exists.
     /// @warning The pipeline must be initialized before calling this function.
     /// @param[in] type type name of managed resource.
     /// @return resource manager on success, nullptr otherwise.
     resources::IResourceManager *get_resource_manager( strings::StaticName type ) noexcept;

     /// @brief Add resource manager.
     /// @warning The pipeline must be initialized before calling this function.
     /// @param[in] type type name of managed resource.
     /// @param[in] manager resource manager.
     /// @return true on success, false otherwise.
     bool add_resource_manager( std::string_view type, resources::IResourceManagerPtr manager );

     /// @brief Get unified storage.
     /// @warning The pipeline must be initialized before calling this function.
     /// @return unified storage.
     resources::UnifiedStorage& get_unified_storage() noexcept;

     /// @brief Get ECS registry.
     /// @warning The pipeline must be initialized before calling this function.
     /// @return ECS registry.
     ecs::EcsRegistry& get_ecs_registry() noexcept;

     /// @brief Get ECS storage which is currently active.
     /// @warning The pipeline must be initialized before calling this function.
     /// @return current ECS storage.
     ecs::EcsStorage& get_current_ecs_storage() noexcept;

     /// @brief Get plugin service locator.
     /// @warning The pipeline must be initialized before calling this function.
     /// @return plugin service locator.
     loader::ServiceLocator& get_service_locator() noexcept;

     /// @brief Get configuration manager.
     /// @warning The pipeline must be initialized before calling this function.
     /// @return configuration manager.
     config::ConfigManager& get_config_manager() noexcept;

private:
     Pipeline( const Pipeline& ) = delete;
     Pipeline& operator=( const Pipeline& ) = delete;

     /// @brief Private implementation of the pipeline.
     class Impl;

private:
     memory::MemoryManager memory_;     ///< memory domain manager.
     Impl *impl_;                       ///< implementation of the pipeline.
};

} // namespace _16nar::profile

#endif // #ifndef _16NAR_CORE_PROFILE_GENERAL_PIPELINE_H
