#include <16nar/platform/strings/name_manager.h>

namespace _16nar::strings
{

NameManager::NameManager( std::pmr::memory_resource& resource ):
     table_names_{ resource },
     tables_{ &resource },
     resource_{ resource }
{}


NameTable *NameManager::add_table( std::string_view name )
{
     const auto key = table_names_.add_name( name );
     if ( key.empty() )
     {
          return nullptr;
     }

     const auto ret = tables_.try_emplace( key, resource_ );
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
