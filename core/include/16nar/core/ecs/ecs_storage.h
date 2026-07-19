/// @file
/// @brief File with EcsStorage class definition.
#ifndef _16NAR_CORE_ECS_ECS_STORAGE_H
#define _16NAR_CORE_ECS_ECS_STORAGE_H

#include <16nar/core/ecs/defs.h>

#include <16nar/platform/strings/name_table.h>
#include <16nar/platform/memory/shared_buffer_ptr.h>
#include <16nar/core/ecs/page_registry.h>

#include <memory_resource>
#include <vector>
#include <unordered_map>
#include <climits>

namespace _16nar::ecs
{

/// @brief Bitmask of a single 256-component block.
struct NARENGINE_CORE_API BlockMask
{
     constexpr static std::size_t mask_size = 4;  ///< count of integers in bitmask.

     constexpr static std::size_t mask_width = sizeof( std::uint64_t ) * CHAR_BIT;   ///< count of bits in bitmask.

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

     /// @brief Binary NOT.
     /// @return Mask with all bits negated.
     BlockMask operator~() const noexcept;

     /// @brief Binary AND with assignment.
     /// @param[in] other right operand.
     /// @return current object after binary AND application.
     BlockMask& operator&=( const BlockMask& other ) noexcept;

     /// @brief Binary OR with assignment.
     /// @param[in] other right operand.
     /// @return current object after binary OR application.
     BlockMask& operator|=( const BlockMask& other ) noexcept;

     std::uint64_t mask[ mask_size ]{};           ///< representation of a bitmask.
};


/// @brief Description of a component type.
struct NARENGINE_CORE_API ComponentDescription
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
class NARENGINE_CORE_API EcsStorage
{
public:
     constexpr static std::size_t entities_per_page = 256;  ///< maximum count of entities per one page.

     friend class Query;

     /// @brief Constructor.
     /// @param[in] type_names table of type and quasitype names.
     /// @param[in] resource memory resource for utility data allocations.
     /// @param[in] big_resource memory resource for ECS data allocations.
     EcsStorage( strings::NameTable& type_names,
          std::pmr::memory_resource& resource,
          std::pmr::memory_resource& big_resource );

     /// @brief Destructor.
     ~EcsStorage();

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

     /// @brief Check if entity with given identifier exists.
     /// @param[in] id identifier of an entity.
     /// @return true if entity exists, false otherwise.
     bool has_entity( EntityId id ) const noexcept;

     /// @brief Add component to an entity.
     /// @details Adding a component which is already added is an error.
     /// Flag components must not be added with this function,
     /// for flags see @b add_flag_component().
     /// @tparam T type of the component.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return pointer to added component on success, nullptr otherwise.
     template < typename T >
     T *add_component( EntityId id, strings::StaticName type )
     {
          return reinterpret_cast< T * >( add_component_raw( id, type ) );
     }

     /// @brief Add flag component to an entity.
     /// @details Unlike with @b add_component() function,
     /// adding a flag component if it's already added is not an error.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return true if the component added successfully, false otherwise.
     bool add_flag_component( EntityId id, strings::StaticName type );

     /// @brief Delete component of an entity with given identifier.
     /// @details This function marks component for deletion, but the deleteion is
     /// performed during the commit (ususally, in the end of the frame), @see commit().
     /// The function does nothing if the component does not exist.
     /// @param[in] id identifier of an entity.
     /// @param[in] type type of the component.
     void delete_component( EntityId id, strings::StaticName type );

     /// @brief Check if entity has component of given type.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return true if component exists, false otherwise.
     bool has_component( EntityId id, strings::StaticName type ) const noexcept;

     /// @brief Get component of an entity.
     /// @tparam T type of the component.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return pointer to the component if it exists, nullptr otherwise.
     template < typename T >
     T *get_component( EntityId id, strings::StaticName type ) noexcept
     {
          return reinterpret_cast< T * >( const_cast< std::byte * >( get_component_raw( id, type ) ) );
     }

     /// @brief Get constant component of an entity.
     /// @tparam T type of the component.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return pointer to the constant component if it exists, nullptr otherwise.
     template < typename T >
     const T *get_component( EntityId id, strings::StaticName type ) const noexcept
     {
          return reinterpret_cast< const T * >( get_component_raw( id, type ) );
     }

     /// @brief Commit delayed structural changes.
     /// @details This function performs deletion of requested entities and components.
     void commit();

     /// @brief Clear all entities and components and page assignments.
     void clear();

     /// @brief Perform a pre-assignment of requested number of pages to a quasitype.
     /// @param[in] quasitype name of the quasitype.
     /// @param[in] count count of pages to be pre-assigned.
     void preassign_pages( strings::StaticName quasitype, std::size_t count );

private:
     EcsStorage( const EcsStorage&& ) = delete;
     EcsStorage& operator=( const EcsStorage& ) = delete;

     /// @brief Inner component type for entity management.
     struct EntityMetadata
     {
          EcsId gen_id{};     ///< generation ID of the ECS world at the moment of entity creation.
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

     /// @brief Add component to an entity without type cast.
     /// @details Adding a component which is already added is an error.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return pointer to added component's raw data on success, nullptr otherwise.
     std::byte *add_component_raw( EntityId id, strings::StaticName type );

     /// @brief Implementation of addition of a component to an entity.
     /// @param[in] desc component type description.
     /// @param[in] page_index index of the page.
     /// @param[in] local_index index of the entity on the page.
     /// @return pointer to added component memory on success, nullptr otherwise.
     std::byte *add_component_impl( ComponentDescription& desc, EcsId page_index, EcsId local_index );

     /// @brief Get raw data of a component of an entity.
     /// @param[in] id identifier of an entity.
     /// @param[in] type name of the component type.
     /// @return pointer to the component's raw data if it exists, nullptr otherwise.
     const std::byte *get_component_raw( EntityId id, strings::StaticName type ) const noexcept;

     /// @brief Set first- and second-order bitmasks.
     /// @param[in] page_index index of the page.
     /// @param[in] local_index index of the entity on the page.
     /// @param[in,out] mask first-order bitmasks.
     /// @param[in,out] block_mask second-order bitmasks.
     void set_masks( EcsId page_index, EcsId local_index,
          std::pmr::vector< BlockMask >& mask,
          std::pmr::vector< std::uint64_t >& block_mask );

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

     /// @brief Clear all components of single type.
     /// @param[in] desc component type description.
     void clear_component_type( ComponentDescription& desc );

     /// @brief Get full entity identifier.
     /// @param[in] page_index index of the page.
     /// @param[in] local_index index of the entity on the page.
     EntityId get_entity_id( EcsId page_index, EcsId local_index ) const noexcept;

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

#endif // #ifndef _16NAR_CORE_ECS_ECS_STORAGE_H
