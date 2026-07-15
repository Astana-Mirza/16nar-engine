#include <16nar/core/ecs/ecs_manager.h>

#include <16nar/platform/logger/logger.h>

#include <limits>
#include <climits>
#include <cassert>

namespace _16nar::ecs
{
namespace
{

constexpr static std::size_t bits_per_mask = sizeof( std::uint64_t ) * CHAR_BIT;

/// @brief Count number of trailing zeroes in a 64-bit number.
/// @param[in] x target number.
/// @return number of trailing zeroes or 0 if @b x is 0.
constexpr unsigned int ntz64( std::uint64_t x ) noexcept
{
    constexpr std::uint64_t debruijn{ 0x022fdd63cc95386d };
    constexpr unsigned int debruijn_table[ 64 ] = {
          0,  1,  2,  53, 3,  7,  54, 27, 4,  38, 41, 8,  34, 55, 48, 28,
          62, 5,  39, 46, 44, 42, 22, 9,  24, 35, 59, 56, 49, 18, 29, 11,
          63, 52, 6,  26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
          51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12
    };
    const auto lsb = x & -x;
    return debruijn_table[ ( lsb * debruijn ) >> 58 ];
}

} // anonymous namespace


EcsManager::EcsManager( strings::NameTable& type_names,
          std::pmr::memory_resource& resource,
          std::pmr::memory_resource& big_resource )
     : components_( &resource )
     , registries_( &resource )
     , pages_( &resource )
     , metadata_{}
     , type_names_{ type_names }
     , resource_{ resource }
     , big_resource_{ big_resource }
{
     // register inner component EntityMetadata with empty type name
     auto iter = components_.emplace( strings::StaticName{}, resource_ ).first;
     metadata_ = &( iter->second );
     metadata_->component_size = sizeof( EntityMetadata );
}


bool EcsManager::register_component_type( strings::StaticName type,
     std::size_t size,
     LifetimeController construct,
     LifetimeController destruct )
{
     if ( type.empty() )
     {
          LOG_16NAR_ERROR( "Cannot register ECS component with empty type name" );
          return false;
     }
     auto [ iter, result ] = components_.try_emplace( type, resource_ );
     if ( !result )
     {
          return false;
     }

     const auto page_count = pages_.size();
     const auto page_mask_count = page_count ?
          ( ( page_count + bits_per_mask - 1 ) / bits_per_mask ) : 1;
     iter->second.present.resize( page_count );
     iter->second.delayed.resize( page_count );
     iter->second.blocks_present.resize( page_mask_count );
     iter->second.blocks_delayed.resize( page_mask_count );
     iter->second.data.resize( page_count );
     iter->second.component_size = size;
     iter->second.construct = construct;
     iter->second.destruct = destruct;

     return true;
}


EntityId EcsManager::add_entity( strings::StaticName quasitype )
{
     if ( quasitype.empty() )
     {
          return EntityId{};
     }

     EcsId page_index{};
     EcsId gen_id{};
     bool new_page = false;
     auto [ iter, result ] = registries_.try_emplace( quasitype, resource_ );
     if ( result || iter->second.empty() )
     {
          new_page = true;
          gen_id = 1;
          page_index = static_cast< EcsId >( pages_.size() );
          pages_.emplace_back( PageInfo{ &( iter->second ), gen_id } );
          adjust_page_masks();
          iter->second.add_page( page_index );
     }
     else
     {
          page_index = iter->second.get_top_page();
          assert( page_index < pages_.size() );
          gen_id = ++pages_[ page_index ].gen_id;
     }

     assert( page_index < metadata_->present.size() );
     const auto local_index = metadata_->present[ page_index ].find_unset();
     assert( local_index < entities_per_page );

     auto *entity = reinterpret_cast< EntityMetadata * >(
          add_component_impl( *metadata_, page_index, local_index ) );
     assert( entity );
     entity->gen_id = gen_id;

     if ( !new_page && metadata_->present[ page_index ].all() )
     {
          // the page is full, remove it from registry
          iter->second.pop_page();
     }

     return EntityId{ gen_id, static_cast< EcsId >( page_index * entities_per_page + local_index ) };
}


void EcsManager::delete_entity( EntityId id )
{
     const EcsId page_index = id.id / entities_per_page;
     const EcsId local_index = id.id % entities_per_page;
     if ( !check_entity( id.gen_id, page_index, local_index ) )
     {
          return;
     }
     set_masks( page_index, local_index, metadata_->delayed, metadata_->blocks_delayed );
     metadata_->has_delayed_changes = true;
     has_delayed_changes_ = true;
}


void EcsManager::delete_component( EntityId id, strings::StaticName type )
{
     const EcsId page_index = id.id / entities_per_page;
     const EcsId local_index = id.id % entities_per_page;
     if ( type.empty() || !check_entity( id.gen_id, page_index, local_index ) )
     {
          return;
     }

     const auto iter = components_.find( type );
     if ( iter == components_.end()
          || !iter->second.present[ page_index ].test( local_index ) )
     {
          return;
     }
     set_masks( page_index, local_index, iter->second.delayed, iter->second.blocks_delayed );
     iter->second.has_delayed_changes = true;
     has_delayed_changes_ = true;
}


void EcsManager::commit()
{
     if ( !has_delayed_changes_ )
     {
          return;
     }
     has_delayed_changes_ = false;

     for ( auto& [ type, desc ] : components_ )
     {
          if ( type.empty() )
          {
               // skip metadata, handle it later
               continue;
          }
          commit_component_type( desc );
     }
     commit_metadata();
}


void EcsManager::preassign_pages( strings::StaticName quasitype, std::size_t count )
{
     count = std::min( std::numeric_limits< EcsId >::max() - pages_.size() - 1, count );
     if ( quasitype.empty() || !count )
     {
          return;
     }
     auto iter = registries_.try_emplace( quasitype, resource_ ).first;
     auto page_index = static_cast< EcsId >( pages_.size() );
     for ( std::size_t i = 0; i < count; ++i )
     {
          pages_.emplace_back( PageInfo{ &( iter->second ), 1 } );
     }
     adjust_page_masks();
     iter->second.add_page_range( page_index, page_index + count );
}


bool EcsManager::check_entity( EcsId gen_id, EcsId page_index, EcsId local_index ) const noexcept
{
     if ( page_index < pages_.size() )
     {
          return false;
     }
     if ( !metadata_->present[ page_index ].test( local_index )
          || metadata_->delayed[ page_index ].test( local_index ) )
     {
          return false;
     }

     auto buffer = metadata_->data[ page_index ].get_view();
     auto entity = reinterpret_cast< EntityMetadata * >( buffer.data + local_index * metadata_->component_size );
     if ( entity->gen_id != gen_id )
     {
          // passed a dangling reference
          return false;
     }
     return true;
}


std::byte *EcsManager::add_component_impl(
     EcsManager::ComponentDescription& desc,
     EcsId page_index, EcsId local_index )
{
     std::byte *result{};
     if ( desc.present[ page_index ].test( local_index ) )
     {
          return result;
     }
     if ( desc.component_size )
     {
          if ( !desc.data[ page_index ] )
          {
               desc.data[ page_index ] = memory::SharedBufferPtr::allocate(
                    big_resource_, entities_per_page * desc.component_size );
          }
          auto buffer = desc.data[ page_index ].get_view();
          result = buffer.data + local_index * desc.component_size;
          if ( desc.construct )
          {
               desc.construct( result );
          }
     }
     set_masks( page_index, local_index, desc.present, desc.blocks_present );
     return result;
}


void EcsManager::set_masks( EcsId page_index, EcsId local_index,
     std::pmr::vector< EcsManager::BlockMask >& mask,
     std::pmr::vector< std::uint64_t > block_mask )
{
     mask[ page_index ].set( local_index );
     block_mask[ page_index / bits_per_mask ] |= ( 1ULL << ( page_index % bits_per_mask ) );
}


void EcsManager::adjust_page_masks()
{
     const auto page_count = pages_.size();
     const auto page_mask_count = page_count ?
          ( ( page_count + bits_per_mask - 1 ) / bits_per_mask ) : 1;
     for ( auto& [ type, desc ] : components_ )
     {
          desc.present.resize( page_count );
          desc.delayed.resize( page_count );
          desc.blocks_present.resize( page_mask_count );
          desc.blocks_delayed.resize( page_mask_count );
          desc.data.resize( page_count );
     }
}


void EcsManager::commit_component_type( EcsManager::ComponentDescription& desc )
{
     if ( !metadata_->has_delayed_changes && !desc.has_delayed_changes )
     {
          // no changes, skip
          return;
     }
     desc.has_delayed_changes = false;
     std::size_t pagemask_index{};
     while ( pagemask_index < metadata_->blocks_present.size() )
     {
          const std::uint64_t orig_block_mask =
               metadata_->blocks_delayed[ pagemask_index ]
               & desc.blocks_present[ pagemask_index ]
               | desc.blocks_delayed[ pagemask_index ];
          auto block_mask = orig_block_mask;
          auto first_page_index = pagemask_index * bits_per_mask;
          while ( block_mask )
          {
               commit_block( desc, first_page_index + ntz64( block_mask ) );
               block_mask &= ( block_mask - 1 );  // clear rightmost set bit
          }
          desc.blocks_present[ pagemask_index ] &= ~orig_block_mask;
          desc.blocks_delayed[ pagemask_index ] = 0;
          ++pagemask_index;
     }
}


void EcsManager::commit_block( EcsManager::ComponentDescription& desc, EcsId page_index )
{
     BlockMask orig_mask = metadata_->delayed[ page_index ];
     orig_mask &= desc.present[ page_index ];
     orig_mask |= desc.delayed[ page_index ];
     if ( desc.component_size && desc.destruct )
     {
          auto mask = orig_mask;
          while ( !mask.none() )
          {
               const auto local_index = mask.find_set();
               desc.destruct( desc.data[ page_index ].get_view().data + local_index * desc.component_size );
               const auto mask_part = local_index / bits_per_mask;
               mask.mask[ mask_part ] &= ( mask.mask[ mask_part ] - 1 ); // clear rightmost set bit
          }
     }
     desc.present[ page_index ] &= ~orig_mask;
     desc.delayed[ page_index ] = BlockMask{};

     if ( desc.present[ page_index ].none() )
     {
          desc.data[ page_index ].reset();
     }
}


void EcsManager::commit_metadata()
{
     if ( !metadata_->has_delayed_changes )
     {
          // no changes, skip
          return;
     }
     metadata_->has_delayed_changes = false;
     std::size_t pagemask_index{};
     while ( pagemask_index < metadata_->blocks_present.size() )
     {
          const std::uint64_t orig_block_mask = metadata_->blocks_delayed[ pagemask_index ];
          auto block_mask = orig_block_mask;
          auto first_page_index = pagemask_index * bits_per_mask;
          while ( block_mask )
          {
               commit_metadata_block( first_page_index + ntz64( block_mask ) );
               block_mask &= ( block_mask - 1 );  // clear rightmost set bit
          }
          metadata_->blocks_present[ pagemask_index ] &= ~orig_block_mask;
          metadata_->blocks_delayed[ pagemask_index ] = 0;
          ++pagemask_index;
     }
}


void EcsManager::commit_metadata_block( EcsId page_index )
{
     BlockMask orig_mask = metadata_->delayed[ page_index ];
     const bool was_full = metadata_->present[ page_index ].all();
     metadata_->present[ page_index ] &= ~orig_mask;
     metadata_->delayed[ page_index ] = BlockMask{};

     if ( metadata_->present[ page_index ].none() )
     {
          metadata_->data[ page_index ].reset();
     }
     if ( was_full )
     {
          // return page back to corresponding registry
          pages_[ page_index ].registry->add_page( page_index );
     }
}


EcsManager::ComponentDescription::ComponentDescription( std::pmr::memory_resource& resource )
     : present( &resource )
     , delayed( &resource )
     , blocks_present( &resource )
     , blocks_delayed( &resource )
     , data( &resource )
     , component_size{}
     , construct{}
     , destruct{}
{}


bool EcsManager::BlockMask::none() const noexcept
{
     return !mask[ 0 ] && !mask[ 1 ] && !mask[ 2 ] && !mask[ 3 ];
}


bool EcsManager::BlockMask::all() const noexcept
{
     return mask[ 0 ] && mask[ 1 ] && mask[ 2 ] && mask[ 3 ];
}


EcsId EcsManager::BlockMask::find_unset() const noexcept
{
     for ( std::size_t i = 0; i < mask_size; ++i )
     {
          if ( mask[ i ] != std::numeric_limits< std::uint64_t >::max() )
          {
               return static_cast< EcsId >( i * bits_per_mask + ntz64( ~mask[ i ] ) );
               break;
          }
     }
     return std::numeric_limits< EcsId >::max();
}


EcsId EcsManager::BlockMask::find_set() const noexcept
{
     for ( std::size_t i = 0; i < mask_size; ++i )
     {
          if ( mask[ i ] )
          {
               return static_cast< EcsId >( i * bits_per_mask + ntz64( mask[ i ] ) );
          }
     }
     return std::numeric_limits< EcsId >::max();
}


bool EcsManager::BlockMask::test( EcsId index ) const noexcept
{
     return mask[ index / bits_per_mask ] & ( 1 << ( index % bits_per_mask ) );
}


void EcsManager::BlockMask::set( EcsId index ) noexcept
{
     mask[ index / bits_per_mask ] |= ( 1 << ( index % bits_per_mask ) );
}


EcsManager::BlockMask EcsManager::BlockMask::operator~() noexcept
{
     BlockMask result;
     result.mask[ 0 ] = ~mask[ 0 ];
     result.mask[ 1 ] = ~mask[ 1 ];
     result.mask[ 2 ] = ~mask[ 2 ];
     result.mask[ 3 ] = ~mask[ 3 ];
     return result;
}


EcsManager::BlockMask& EcsManager::BlockMask::operator&=( const EcsManager::BlockMask& other ) noexcept
{
     mask[ 0 ] &= other.mask[ 0 ];
     mask[ 1 ] &= other.mask[ 1 ];
     mask[ 2 ] &= other.mask[ 2 ];
     mask[ 3 ] &= other.mask[ 3 ];
     return *this;
}


EcsManager::BlockMask& EcsManager::BlockMask::operator|=( const EcsManager::BlockMask& other ) noexcept
{
     mask[ 0 ] |= other.mask[ 0 ];
     mask[ 1 ] |= other.mask[ 1 ];
     mask[ 2 ] |= other.mask[ 2 ];
     mask[ 3 ] |= other.mask[ 3 ];
     return *this;
}

} // namespace _16nar::ecs
