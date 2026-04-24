/// @file
/// @brief File with StaticName structure definition.
#ifndef _16NAR_TOOLS_STATIC_NAME_H
#define _16NAR_TOOLS_STATIC_NAME_H

#include <16nar/tools/defs.h>
#include <16nar/tools/misc/hash.h>

namespace _16nar::tools
{

/// @brief Name which is used as hash in runtime.
struct StaticName
{
     /// @brief Default constructor.
     constexpr StaticName() noexcept
          :hash{}
     {}

     /// @brief Constructor.
     /// @param[in] name name to be hashed.
     constexpr explicit StaticName( std::string_view name ) noexcept
          :hash{ str_hash64( name ) }
     {}

     /// @brief Check if current name is empty.
     /// @return true if the name is empty, false otherwise.
     constexpr bool empty() const noexcept
     {
          return !hash;
     }

     constexpr bool operator<(  StaticName rhs ) const noexcept { return hash <  rhs.hash; }
     constexpr bool operator>(  StaticName rhs ) const noexcept { return hash >  rhs.hash; }
     constexpr bool operator<=( StaticName rhs ) const noexcept { return hash <= rhs.hash; }
     constexpr bool operator>=( StaticName rhs ) const noexcept { return hash >= rhs.hash; }
     constexpr bool operator==( StaticName rhs ) const noexcept { return hash == rhs.hash; }
     constexpr bool operator!=( StaticName rhs ) const noexcept { return hash != rhs.hash; }

     std::uint64_t hash{};    ///< hash of the name.
};

} // namespace _16nar::tools


namespace std
{

/// @brief Definition of hash for StaticName.
template<>
struct hash< _16nar::tools::StaticName >
{
     constexpr size_t operator()( _16nar::tools::StaticName name ) const noexcept
     {
          return name.hash;
     }
};

} // namespace std

#endif // #ifndef _16NAR_TOOLS_STATIC_NAME_H
