#include <16nar/core/ecs/query.h>

#include <16nar/platform/math/bits.h>
#include <16nar/platform/logger/logger.h>

#include <16nar/core/ecs/ecs_registry.h>

#include <limits>

namespace _16nar::ecs
{

Query::Query( EcsStorage& storage, std::pmr::memory_resource& resource )
     : elements_( &resource )
     , result_{ *this }
     , storage_{ storage }
     , valid_{ true }
     , finished_{ true }
{}


Query& Query::has( strings::StaticName type )
{
     if ( !finished_ )
     {
          LOG_16NAR_ERROR( "Cannot modify ECS query while being executed" );
     }
     ComponentDescription *desc{};
     if ( !get_description( type, false, desc ) )
     {
          valid_ = false;
          return *this;
     }
     auto& elem = elements_.emplace_back();
     elem.type = type;
     elem.desc = desc;
     elem.data = nullptr;
     elem.flags = QueryElement::SearchModifier | QueryElement::Wanted;
     elem.found = false;
     return *this;
}


Query& Query::has_not( strings::StaticName type )
{
     if ( !finished_ )
     {
          LOG_16NAR_ERROR( "Cannot modify ECS query while being executed" );
     }
     ComponentDescription *desc{};
     if ( !get_description( type, false, desc ) )
     {
          valid_ = false;
          return *this;
     }
     auto& elem = elements_.emplace_back();
     elem.type = type;
     elem.desc = desc;
     elem.data = nullptr;
     elem.flags = QueryElement::SearchModifier;
     elem.found = false;
     return *this;
}


Query& Query::get( strings::StaticName type, std::size_t& index )
{
     if ( !finished_ )
     {
          LOG_16NAR_ERROR( "Cannot modify ECS query while being executed" );
     }
     ComponentDescription *desc{};
     if ( !get_description( type, true, desc ) )
     {
          valid_ = false;
          return *this;
     }
     index = elements_.size();
     auto& elem = elements_.emplace_back();
     elem.type = type;
     elem.desc = desc;
     elem.data = nullptr;
     elem.flags = QueryElement::SearchModifier
          | QueryElement::Wanted | QueryElement::DataWanted;
     elem.found = false;
     return *this;
}


Query& Query::get_optional( strings::StaticName type, std::size_t& index )
{
     if ( !finished_ )
     {
          LOG_16NAR_ERROR( "Cannot modify ECS query while being executed" );
     }
     ComponentDescription *desc{};
     if ( !get_description( type, true, desc ) )
     {
          valid_ = false;
          return *this;
     }
     index = elements_.size();
     auto& elem = elements_.emplace_back();
     elem.type = type;
     elem.desc = desc;
     elem.data = nullptr;
     elem.flags = QueryElement::Wanted | QueryElement::DataWanted;
     elem.found = false;
     return *this;
}


Query& Query::has_optional( strings::StaticName type, std::size_t& index )
{
     if ( !finished_ )
     {
          LOG_16NAR_ERROR( "Cannot modify ECS query while being executed" );
     }
     ComponentDescription *desc{};
     if ( !get_description( type, false, desc ) )
     {
          valid_ = false;
          return *this;
     }
     index = elements_.size();
     auto& elem = elements_.emplace_back();
     elem.type = type;
     elem.desc = desc;
     elem.data = nullptr;
     elem.flags = QueryElement::Wanted;
     elem.found = false;
     return *this;
}


bool Query::valid() const noexcept
{
     return valid_;
}


bool Query::finished() const noexcept
{
     return finished_;
}


Query::Result& Query::execute() noexcept
{
     result_.reset();
     if ( !valid() || !preprocess() )
     {
          return result_;
     }
     finished_ = false;
     advance();
     return result_;
}


Query::Result& Query::get_result() noexcept
{
     return result_;
}


const Query::Result& Query::get_result() const noexcept
{
     return result_;
}


void Query::reset()
{
     elements_.clear();
     result_.reset();
     valid_ = true;
     finished_ = true;
}


bool Query::get_description( strings::StaticName type,
     bool data_wanted, ComponentDescription *& desc )
{
     desc = nullptr;
     TypeDescription *type_desc{};
     auto iter = storage_.components_.find( type );
     if ( iter == storage_.components_.end() )
     {
          type_desc = storage_.ecs_registry_.get_component_type( type );
          if ( !type_desc )
          {
               const auto type_name = storage_.type_names_.get_name( type, true );
               LOG_16NAR_ERROR( "Cannot prepare ECS query, component '%.*s' not found",
                    static_cast< int >( type_name.size() ), type_name.data() );
               return false;
          }

          // leave desc = nullptr, try to fill it in preprocess()
     }
     else
     {
          desc = &iter->second;
          type_desc = &desc->type;
     }

     if ( data_wanted && !type_desc->size )
     {
          const auto type_name = storage_.type_names_.get_name( type, true );
          LOG_16NAR_ERROR( "Cannot prepare ECS query, attempt to get data of a flag type '%.*s'",
               static_cast< int >( type_name.size() ), type_name.data() );
          return false;
     }
     return true;
}


bool Query::preprocess()
{
     for ( auto& elem : elements_ )
     {
          elem.found = false;
          if ( elem.desc )
          {
               continue;
          }

          auto iter = storage_.components_.find( elem.type );
          if ( iter != storage_.components_.end() )
          {
               elem.desc = &iter->second;
               continue;
          }

          if ( elem.flags & ( QueryElement::SearchModifier | QueryElement::Wanted ) )
          {
               // required component does not exist, query may be omitted
               return false;
          }
     }
     return true;
}


void Query::advance() noexcept
{
     while ( !finished_ )
     {
          // search next entity on the page
          result_.local_index_ = result_.page_.find_set();
          if ( result_.local_index_ != std::numeric_limits< EcsId >::max() )
          {
               const auto mask_part = result_.local_index_ / BlockMask::mask_width;
               result_.page_.mask[ mask_part ] &= ( result_.page_.mask[ mask_part ] - 1 );
               fill_result();
               return;
          }

          // search next page in the page block
          if ( result_.page_block_ )
          {
               result_.page_index_ = ( result_.block_index_ - 1 ) * BlockMask::mask_width
                    + math::ntz64( result_.page_block_ );
               result_.page_block_ &= ( result_.page_block_ - 1 );
               result_.page_ = storage_.metadata_->present[ result_.page_index_ ];
               result_.page_ &= ~storage_.metadata_->delayed[ result_.page_index_ ];
               for ( auto& elem : elements_ )
               {
                    if ( !elem.desc || !( elem.flags & QueryElement::SearchModifier ) )
                    {
                         continue;
                    }
                    const auto present = elem.desc->present[ result_.page_index_ ];
                    const auto delayed = elem.desc->delayed[ result_.page_index_ ];
                    if ( elem.flags & QueryElement::Wanted )
                    {
                         result_.page_ &= present;
                         result_.page_ &= ~delayed;
                    }
                    else
                    {
                         result_.page_ &= ~present;
                         result_.page_ &= delayed;
                    }
               }
               continue;
          }

          // search next page block
          const auto max_blocks = storage_.metadata_->blocks_present.size();
          for ( ; !result_.page_block_ && result_.block_index_ < max_blocks; ++result_.block_index_ )
          {
               result_.page_block_ = storage_.metadata_->blocks_present[ result_.block_index_ ];
               result_.page_block_ &= ~storage_.metadata_->blocks_delayed[ result_.block_index_ ];
               for ( auto& elem : elements_ )
               {
                    if ( !elem.desc || !( elem.flags & QueryElement::SearchModifier ) )
                    {
                         continue;
                    }
                    const auto present = elem.desc->blocks_present[ result_.block_index_ ];
                    const auto delayed = elem.desc->blocks_delayed[ result_.block_index_ ];
                    if ( elem.flags & QueryElement::Wanted )
                    {
                         result_.page_block_ &= present;
                         result_.page_block_ &= ~delayed;
                    }
                    else
                    {
                         result_.page_block_ &= ~present;
                         result_.page_block_ &= delayed;
                    }
               }
          }
          if ( result_.page_block_ )
          {
               continue;
          }

          // query finished, storage traversed
          result_.reset();
          finished_ = true;
     }
}


void Query::fill_result()
{
     for ( auto& elem : elements_ )
     {
          if ( !elem.desc )
          {
               elem.found = false;
               elem.data = nullptr;
               continue;
          }

          if ( elem.flags & QueryElement::SearchModifier )
          {
               elem.found = elem.flags & QueryElement::Wanted;
               if ( elem.flags & QueryElement::DataWanted )
               {
                    auto buffer = elem.desc->data[ result_.page_index_ ].get_view();
                    elem.data = buffer.data + result_.local_index_ * elem.desc->type.size;
               }
               else
               {
                    elem.data = nullptr;
               }
          }
          else
          {
               elem.found = elem.desc->present[ result_.page_index_ ].test( result_.local_index_ )
                    && !elem.desc->delayed[ result_.page_index_ ].test( result_.local_index_ );
               if ( elem.found && ( elem.flags & QueryElement::DataWanted ) )
               {
                    auto buffer = elem.desc->data[ result_.page_index_ ].get_view();
                    elem.data = buffer.data + result_.local_index_ * elem.desc->type.size;
               }
               else
               {
                    elem.data = nullptr;
               }
          }
     }
}


EntityId Query::get_entity_id() const noexcept
{
     return finished_ ? EntityId{}
          : storage_.get_entity_id( result_.page_index_, result_.local_index_ );
}


Query::Iterator::Iterator( Query::Result *result ) noexcept
     : result_{ result }
{}


Query::Result& Query::Iterator::operator*() const noexcept
{
     return *result_;
}


Query::Iterator& Query::Iterator::operator++() noexcept
{
     if ( result_ )
     {
          *this = result_->next();
     }
     return *this;
}


bool Query::Iterator::operator==( const Query::Iterator& other ) const noexcept
{
     return result_ == other.result_;
}


bool Query::Iterator::operator!=( const Query::Iterator& other ) const noexcept
{
     return result_ != other.result_;
}


Query::Result::Result( Query& query ) noexcept
     : query_{ query }
     , page_block_{}
     , page_{}
     , block_index_{}
     , page_index_{}
     , local_index_{}
{}


Query::Iterator Query::Result::begin() noexcept
{
     return Iterator{ query_.finished() ? nullptr : this };
}


Query::Iterator Query::Result::end() noexcept
{
     return Iterator{ nullptr };
}


bool Query::Result::has_component( std::size_t index ) const noexcept
{
     return ( !query_.finished() && index < query_.elements_.size() )
          ? query_.elements_[ index ].found : false;
}


EntityId Query::Result::get_entity_id() const noexcept
{
     return query_.get_entity_id();
}


Query::Iterator Query::Result::next() noexcept
{
     query_.advance();
     return Iterator{ query_.finished() ? nullptr : this };
}


void Query::Result::reset() noexcept
{
     page_block_ = 0;
     page_ = BlockMask{};
     block_index_ = 0;
     page_index_ = 0;
     local_index_ = 0;
}


const std::byte *Query::Result::get_component_impl( std::size_t index ) const noexcept
{
     return ( !query_.finished() && index < query_.elements_.size() )
          ? query_.elements_[ index ].data : nullptr;
}

} // namespace _16nar::ecs
