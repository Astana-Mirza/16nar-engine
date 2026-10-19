/// @file
/// @brief File with EcsRegistry class definition.
#ifndef _16NAR_CORE_ECS_ECS_REGISTRY_H
#define _16NAR_CORE_ECS_ECS_REGISTRY_H

#include <16nar/core/ecs/defs.h>

#include <16nar/platform/strings/name_table.h>

#include <unordered_map>

namespace _16nar::ecs
{

/// @brief Function for construction or destruction of a component at given memory.
using LifetimeController = void ( * )( std::byte * );

/// @brief Description of a component type.
struct TypeDescription
{
     std::size_t size{};                ///< size of one component, in bytes.
     LifetimeController construct{};    ///< function of component construction.
     LifetimeController destruct{};     ///< function of component destruction.
};


/// @brief Registry of ECS component types.
class NARENGINE_CORE_API EcsRegistry
{
public:
     /// @brief Constructor.
     /// @param[in] type_names table of component type names.
     /// @param[in] resource memory resource for utility data allocations.
     EcsRegistry( strings::NameTable& type_names,
          std::pmr::memory_resource& resource );

     /// @brief Register component type for ECS.
     /// @param[in] type name of component type.
     /// @param[in] desc description of component type.
     /// @return true on success, false otherwise.
     bool register_component_type( strings::StaticName type, const TypeDescription& desc );

     /// @brief Get description of component type.
     /// @param[in] type name of component type.
     /// @return description of component type if it exists, nullptr otherwise.
     TypeDescription *get_component_type( strings::StaticName type );

private:
     /// @brief Descriptions of component types with their names.
     using TypeMap = std::pmr::unordered_map< strings::StaticName, TypeDescription >;

     strings::NameTable& type_names_;   ///< ///< table of type names.
     TypeMap types_;                    ///< descriptions of component types.
};

} // namespace _16nar::ecs

#endif // #ifndef _16NAR_CORE_ECS_ECS_REGISTRY_H
