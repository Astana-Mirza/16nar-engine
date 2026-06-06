#include <16nar/platform/strings/name_table.h>

#include <16nar/platform/logger/logger.h>

#include <cstdio>
#include <cinttypes>

namespace _16nar::strings
{
namespace
{

static thread_local char pretty_print_buffer[21]{};

} // anonymous namespace


NameTable::NameTable( memory::MemoryDomain& domain ):
     table_{ &domain.get_resource() }
{}


StaticName NameTable::get_static_name( std::string_view name ) const
{
     if ( name.empty() )
     {
          return StaticName{};
     }

     const StaticName key{ name };
     if ( key.empty() )
     {
          LOG_16NAR_ERROR( "String '%.*s' has zero hash",
               static_cast< int >( name.size() ), name.data() );
          return StaticName{};
     }

     const auto iter = table_.find( key );
     if ( iter == table_.cend() )
     {
          return StaticName{};
     }
     else if ( iter->second != name )
     {
          LOG_16NAR_ERROR( "Hash collision detected for strings '%.*s' and '%.*s'",
               static_cast< int >( iter->second.size() ), iter->second.c_str(),
               static_cast< int >( name.size() ), name.data() );
          return StaticName{};
     }

     return key;
}


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
     if ( name.empty() )
     {
          return StaticName{};
     }

     const StaticName key{ name };
     if ( key.empty() )
     {
          LOG_16NAR_ERROR( "String '%.*s' has zero hash",
               static_cast< int >( name.size() ), name.data() );
          return StaticName{};
     }

     const auto ret = table_.try_emplace( key, name );
     if ( !ret.second )
     {
          const auto& prevented = ret.first->second;
          if ( prevented != name )
          {
               LOG_16NAR_ERROR( "Hash collision detected for strings '%.*s' and '%.*s'",
                    static_cast< int >( prevented.size() ), prevented.c_str(),
                    static_cast< int >( name.size() ), name.data() );
          }
          return StaticName{};
     }
     return key;
}


void NameTable::remove_name( StaticName name )
{
     table_.erase( name );
}

} // namespace _16nar::strings
