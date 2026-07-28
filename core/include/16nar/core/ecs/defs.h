/// @file
/// @brief File with basic definitions for ECS.
#ifndef _16NAR_CORE_ECS_DEFS_H
#define _16NAR_CORE_ECS_DEFS_H

#include <16nar/core/defs.h>

#include <16nar/platform/strings/static_name.h>

namespace _16nar::ecs
{

/// @brief Function for construction or destruction of a component at given memory.
using LifetimeController = void ( * )( std::byte * );

/// @brief Type of identifier in ECS.
using EcsId = std::uint32_t;

/// @brief Identifier of an entity.
/// @details The ECS has counter called generation ID.
/// It is initially set to 1 and incremented when an entity is added.
/// The generation ID is saved with the entity in order to ensure
/// dangling reference safety.
struct alignas( std::uint64_t ) EntityId
{
     /// @brief Bool cast operator.
     /// @return true if identifier is valid, false otherwise.
     explicit constexpr operator bool() const noexcept
     {
          return gen_id;
     }

     /// @brief Equality comparison operator.
     /// @param[in] rhs right operand.
     /// @return true if the operands are equal, false otherwise.
     constexpr bool operator==( EntityId rhs ) const noexcept
     {
          return gen_id == rhs.gen_id && id == rhs.id;
     }

     /// @brief Inequality comparison operator.
     /// @param[in] rhs right operand.
     /// @return true if the operands are equal, false otherwise.
     constexpr bool operator!=( EntityId rhs ) const noexcept
     {
          return gen_id != rhs.gen_id || id != rhs.id;
     }

     EcsId gen_id{};     ///< generation ID of the entity.
     EcsId id{};         ///< identifier of the node in the world.
};

} // namespace _16nar::ecs

#endif // #ifndef _16NAR_CORE_ECS_DEFS_H
