/// @file
/// @brief File with EcsManager class definition.
#ifndef _16NAR_CORE_ECS_ECS_MANAGER_H
#define _16NAR_CORE_ECS_ECS_MANAGER_H

#include <16nar/core/ecs/defs.h>

#include <16nar/platform/strings/name_table.h>
#include <16nar/platform/memory/shared_buffer_ptr.h>
#include <16nar/core/ecs/page_registry.h>

#include <memory_resource>
#include <vector>
#include <unordered_map>

namespace _16nar::ecs
{

/// @brief Manager of Entity-Component-System framework.
/// @details Components in the ECS are allocated in blocks of continuous memory.
/// Each block is sized exactly for 256 components of selected component type.
/// Entities having multiple components will use parallel blocks. Such set of
/// parallel blocks (containing up to 256 entities) is called a page. Note that
/// memory for different blocks in one page is not continuous, unlike memory for
/// a block.
///
/// ECS is based on quasitypes assigned to pages. These are neither types nor archetypes.
/// Quasitype acts like a tag which defines intention to group entities with common
/// component types on the same pages. This grouping makes memory allocations
/// more efficient by leaving less unused memory. But quasitypes are not restrictive,
/// so it is normal to add any component to any entity, although it may lead to
/// some memory waste, i.e. only a small part of allocated memory block will be
/// really used. Thus, efficiency is left up to user.
///
/// When a new entity is created, a suitable page is selected for it depending on
/// its quasitype. User can explicitly request pre-assignment of desired number of
/// pages with desired quasitype.
///
/// Queries to the ECS are designed with use of bitmasks. There is a bitmask
/// allocated for each component type (even if there are no actual blocks allocated
/// for the component). There are also second-order bitmasks with each bit set to 1
/// if any of 256 first-order bitmask bits is 1. This mechanism provides fast scan of
/// the world during a query which looks for components presence or absence.
///
/// ECS permits use of components with zero size, such components are called flag
/// components. In this case, only a bitmask is allocated and the component presence
/// is used as a flag.
///
/// ECS uses inner component type called EntityMetadata. It indicates that the entity
/// exists and stores its generation ID for dangling reference safety.
class NARENGINE_CORE_API EcsManager
{
public:
     constexpr static std::size_t entities_per_page = 256;  ///< maximum count of entities per one page.

     /// @brief Constructor.
     /// @param[in] type_names table of type and quasitype names.
     /// @param[in] resource memory resource for utility data allocations.
     /// @param[in] big_resource memory resource for ECS data allocations.
     EcsManager( strings::NameTable& type_names,
          std::pmr::memory_resource& resource,
          std::pmr::memory_resource& big_resource );

     /// @brief Register component type for ECS.
     /// @param[in] type name of component type.
     /// @param[in] size size of single component object, may be zero for flag components.
     /// @param[in] construct function of component construction, only for non-flag components.
     /// @param[in] destruct function of component destruction, only for non-flag components.
     /// @return true on success, false otherwise.
     bool register_component_type( strings::StaticName type,
          std::size_t size = 0,
          LifetimeController construct = nullptr,
          LifetimeController destruct = nullptr );

     /// @brief Add entity of selected quasitype.
     /// @param[in] quasitype name of the quasitype of added entity.
     /// @return identifier of added entity on success, empty identifier otherwise.
     EntityId add_entity( strings::StaticName quasitype );

     /// @brief Delete entity with given identifier and all its components.
     /// @details This function marks entity for deletion, but the deleteion is
     /// performed during the commit (ususally, in the end of the frame), @see commit().
     /// The function does nothing if the entity does not exist.
     /// @param[in] id identifier of an entity.
     void delete_entity( EntityId id );

     /// @brief Add component to an entity.
     /// @tparam T type of the component.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return pointer to added component on success, nullptr otherwise.
     template < typename T >
     T *add_component( EntityId id, strings::StaticName type )
     {
          const EcsId page_index = id.id / entities_per_page;
          const EcsId local_index = id.id % entities_per_page;
          if ( type.empty() || !check_entity( id.gen_id, page_index, local_index ) )
          {
               return nullptr;
          }
          const auto iter = components_.find( type );
          if ( iter == components_.end() )
          {
               return nullptr;
          }
          return reinterpret_cast< T * >(
               add_component_impl( iter->second, page_index, local_index ) );
     }

     /// @brief Delete component of an entity with given identifier.
     /// @details This function marks component for deletion, but the deleteion is
     /// performed during the commit (ususally, in the end of the frame), @see commit().
     /// The function does nothing if the component does not exist.
     /// @param[in] id identifier of an entity.
     /// @param[in] type type of the component.
     void delete_component( EntityId id, strings::StaticName type );

     /// @brief Commit delayed structural changes.
     /// @details This function performs deletion of requested entities and components.
     void commit();

     /// @brief Perform a pre-assignment of requested number of pages to a quasitype.
     /// @param[in] quasitype name of the quasitype.
     /// @param[in] count count of pages to be pre-assigned.
     void preassign_pages( strings::StaticName quasitype, std::size_t count );

private:
     EcsManager( const EcsManager&& ) = delete;
     EcsManager& operator=( const EcsManager& ) = delete;

     /// @brief Inner component type for entity management.
     struct EntityMetadata
     {
          EcsId gen_id{};     ///< generation ID of the ECS world at the moment of entity creation.
     };

     /// @brief Bitmask of a single 256-component block.
     struct BlockMask
     {
          constexpr static std::size_t mask_size = 4;  ///< count of integers in bitmask.

          /// @brief Check if no bits of the block are set.
          /// @return true if the block is empty, false otherwise.
          bool none() const noexcept;

          /// @brief Check if all bits of the block are set.
          /// @return true if the block is full, false otherwise.
          bool all() const noexcept;

          /// @brief Find index of the first unset bit of the block.
          /// @return index of the first unset bit of the block, -1 if not found.
          EcsId find_unset() const noexcept;

          /// @brief Find index of the first set bit of the block.
          /// @return index of the first set bit of the block, -1 if not found.
          EcsId find_set() const noexcept;

          /// @brief Test if specific bit is set.
          /// @param[in] index index of the bit within the block.
          /// @return true if the bit is set, false otherwise.
          bool test( EcsId index ) const noexcept;

          /// @brief Set specific bit.
          /// @param[in] index index of the bit within the block.
          void set( EcsId index ) noexcept;

          BlockMask operator~() noexcept;
          BlockMask& operator&=( const BlockMask& other ) noexcept;
          BlockMask& operator|=( const BlockMask& other ) noexcept;

          std::uint64_t mask[ mask_size ]{};           ///< representation of a bitmask.
     };

     /// @brief Description of a component type.
     struct ComponentDescription
     {
          /// @brief Constructor.
          /// @param[in] resource memory resource for utility data allocations.
          ComponentDescription( std::pmr::memory_resource& resource );

          std::pmr::vector< BlockMask > present;            ///< bitmasks of component presence.
          std::pmr::vector< BlockMask > delayed;            ///< bitmasks of component delayed changes.
          std::pmr::vector< std::uint64_t > blocks_present; ///< second-order bitmasks of component presence.
          std::pmr::vector< std::uint64_t > blocks_delayed; ///< second-order bitmasks of component delayed changes.
          std::pmr::vector< memory::SharedBufferPtr > data; ///< data of allocated blocks.
          std::size_t component_size;                       ///< size of one component, in bytes.
          LifetimeController construct;                     ///< function of component construction.
          LifetimeController destruct;                      ///< function of component destruction.
          bool has_delayed_changes;                         ///< flag of delayed changes.
     };

     /// @brief Information about single page.
     struct PageInfo
     {
          PageRegistry *registry{};     ///< page registry managing the page.
          EcsId gen_id{};               ///< generation ID of the page.
     };

     /// @brief Page registries with quasitype names.
     using RegistryMap = std::pmr::unordered_map< strings::StaticName, PageRegistry >;

     /// @brief Descriptions of components with their type names.
     using ComponentMap = std::pmr::unordered_map< strings::StaticName, ComponentDescription >;

     /// @brief Check if entity with given parameters exist.
     /// @param[in] gen_id generation ID of the entity.
     /// @param[in] page_index index of the page.
     /// @param[in] local_index index of the entity on the page.
     /// @return true if entity exists, false otherwise.
     bool check_entity( EcsId gen_id, EcsId page_index, EcsId local_index ) const noexcept;

     /// @brief Implementation of addition of a component to an entity.
     /// @param[in] desc component type description.
     /// @param[in] page_index index of the page.
     /// @param[in] local_index index of the entity on the page.
     /// @return pointer to added component memory on success, nullptr otherwise.
     std::byte *add_component_impl( ComponentDescription& desc, EcsId page_index, EcsId local_index );

     /// @brief Set first- and second-order bitmasks.
     /// @param[in] page_index index of the page.
     /// @param[in] local_index index of the entity on the page.
     /// @param[in,out] mask first-order bitmasks.
     /// @param[in,out] block_mask second-order bitmasks.
     void set_masks( EcsId page_index, EcsId local_index,
          std::pmr::vector< BlockMask >& mask,
          std::pmr::vector< std::uint64_t > block_mask );

     /// @brief Resize page bitmasks according to updated page count.
     void adjust_page_masks();

     /// @brief Commit delayed structural changes for a component type.
     /// @param[in] desc component type description.
     void commit_component_type( ComponentDescription& desc );

     /// @brief Commit delayed structural changes for a block.
     /// @param[in] desc component type description.
     /// @param[in] page_index index of the page.
     void commit_block( ComponentDescription& desc, EcsId page_index );

     /// @brief Commit delayed structural changes for metadata component type.
     void commit_metadata();

     /// @brief Commit delayed structural changes for a metadata block.
     /// @param[in] page_index index of the page.
     void commit_metadata_block( EcsId page_index );

private:
     ComponentMap components_;                    ///< description of all component types.
     RegistryMap registries_;                     ///< registries of pages per quasitype.
     std::pmr::vector< PageInfo > pages_;         ///< information about each page.
     ComponentDescription *metadata_;             ///< metadata component description.
     strings::NameTable& type_names_;             ///< table of type and quasitype names.
     std::pmr::memory_resource& resource_;        ///< memory resource for utility data allocations.
     std::pmr::memory_resource& big_resource_;    ///< memory resource for ECS data allocations.
     bool has_delayed_changes_;                   ///< flag of any delayed changes.
};

} // namespace _16nar::ecs

#endif // #ifndef _16NAR_CORE_ECS_ECS_MANAGER_H
