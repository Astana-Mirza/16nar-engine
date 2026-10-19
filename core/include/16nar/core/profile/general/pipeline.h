/// @file
/// @brief File with Pipeline class definition.
#ifndef _16NAR_CORE_PROFILE_GENERAL_PIPELINE_H
#define _16NAR_CORE_PROFILE_GENERAL_PIPELINE_H

#include <16nar/core/defs.h>

#include <16nar/platform/memory/memory_manager.h>
#include <16nar/platform/strings/name_manager.h>

#include <16nar/core/resources/defs.h>
#include <16nar/core/resources/unified_storage.h>
#include <16nar/core/ecs/ecs_storage.h>

#include <unordered_map>

namespace _16nar::profile
{

/// @brief Framework class which implements application execution pipeline.
class NARENGINE_CORE_API Pipeline
{
public:
     /// @brief Constructor.
     /// @param[in] root_domain root memory domain.
     Pipeline( memory::IMemoryDomain& root_domain );

private:
     /// @brief Map of resource managers.
     using ResourceManagers = std::pmr::unordered_map<
          strings::StaticName, resources::IResourceManagerPtr >;

     memory::MemoryManager memory_;     ///< memory domain manager.
     strings::NameManager names_;       ///< name table manager.

     ResourceManagers resources_;       ///< resource managers.
     ecs::EcsStorage ecs_storage_;      ///< storage of ECS data.
};

} // namespace _16nar::profile

#endif // #ifndef _16NAR_CORE_PROFILE_GENERAL_PIPELINE_H
