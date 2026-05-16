#include <16nar/platform/strings/name_table.h>

#include <cstdio>
#include <cinttypes>

namespace _16nar::strings
{
namespace
{

static thread_local char pretty_print_buffer[21]{};

} // anonymous namespace


std::string_view NameTable::get_name( StaticName name, bool pretty ) const noexcept
{
     const auto iter = table_.find( name );
     if ( iter == table_.cend() )
     {
          if ( pretty )
          {
               std::snprintf( pretty_print_buffer, sizeof( pretty_print_buffer ),
                    "[0x%016" PRIx64 "]", name.hash );
               return pretty_print_buffer;
          }
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

} // namespace _16nar::strings
