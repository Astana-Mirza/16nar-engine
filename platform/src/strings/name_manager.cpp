#include <16nar/platform/strings/name_manager.h>

namespace _16nar::strings
{

NameManager::NameManager( memory::MemoryDomain& domain ):
     table_names_{ domain },
     tables_{ &domain.get_resource() },
     domain_{ domain }
{}


NameTable *NameManager::add_table( std::string_view name )
{
     const auto key = table_names_.add_name( name );
     if ( key.empty() )
     {
          return nullptr;
     }

     const auto ret = tables_.try_emplace( key, domain_ );
     return ret.second ? &( ret.first->second ) : nullptr;
}


const NameTable *NameManager::get_table( std::string_view name ) const
{
     // search in table is required to prevent hash collision
     const auto key = table_names_.get_static_name( name );
     if ( key.empty() )
     {
          return nullptr;
     }

     const auto iter = tables_.find( key );
     return ( iter == tables_.end() ) ? nullptr : &( iter->second );
}


NameTable *NameManager::get_table( std::string_view name )
{
     // search in table is required to prevent hash collision
     const auto key = table_names_.get_static_name( name );
     if ( key.empty() )
     {
          return nullptr;
     }

     const auto iter = tables_.find( key );
     return ( iter == tables_.end() ) ? nullptr : &( iter->second );
}


void NameManager::remove_table( std::string_view name )
{
     // search in table is required to prevent hash collision
     const auto key = table_names_.get_static_name( name );
     if ( key.empty() )
     {
          return;
     }

     table_names_.remove_name( key );
     tables_.erase( key );
}

} // namespace _16nar::strings
