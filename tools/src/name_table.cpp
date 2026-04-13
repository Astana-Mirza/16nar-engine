#include <16nar/tools/name_table.h>

namespace _16nar::tools
{

std::string_view NameTable::get_name( StaticName name ) const noexcept
{
     const StaticName key{ name };
     const auto iter = table_.find( key );
     if ( iter == table_.cend() )
     {
          return std::string_view{};
     }
     return std::string_view{ iter->second.data(), iter->second.size() };
}


StaticName NameTable::add_name( std::string_view name )
{
     const StaticName key{ name };
     if ( name.empty() || key.empty() || !table_.try_emplace( key, name ).second )
     {
          return StaticName{};
     }
     return key;
}


void NameTable::remove_name( StaticName name )
{
     table_.erase( name );
}

} // namespace _16nar::tools
