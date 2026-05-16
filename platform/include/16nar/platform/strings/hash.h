/// @file
/// @brief Hash functions used in the engine.
#ifndef _16NAR_PLATFORM_STRINGS_HASH_H
#define _16NAR_PLATFORM_STRINGS_HASH_H

#include <16nar/platform/defs.h>

#include <cstddef>
#include <string_view>

namespace _16nar::strings
{

/// @brief Calculate 32-bit hash for given string, use FNV-1a hash algorithm.
/// @param[in] str string to be hashed.
/// @return 32-bit hash.
constexpr std::uint32_t str_hash32( std::string_view str ) noexcept
{
     std::uint32_t hash{ 0x811c9dc5 };
     for ( const auto c : str )
     {
          hash ^= c;
          hash *= 0x1000193;
     }
     return hash;
}


/// @brief Calculate 64-bit hash for given string, use FNV-1a hash algorithm.
/// @param[in] str string to be hashed.
/// @return 64-bit hash.
constexpr std::uint64_t str_hash64( std::string_view str ) noexcept
{
     std::uint64_t hash{ 0xcbf29ce484222325 };
     for ( const auto c : str )
     {
          hash ^= c;
          hash *= 0x100000001b3;
     }
     return hash;
}

} // namespace _16nar::strings

#endif // #ifndef _16NAR_PLATFORM_STRINGS_HASH_H
