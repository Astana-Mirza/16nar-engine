/// @file
/// @brief File with basic definitions for tools library.
#ifndef _16NAR_TOOLS_DEFS_H
#define _16NAR_TOOLS_DEFS_H

#include <16nar/16nardefs.h>

#include <memory>

namespace _16nar::tools
{

/// @brief Identifier of action of asset parsing.
using AssetActionId = std::uint32_t;

/// @brief Identifier of asset content type.
using ContentTypeId = std::uint32_t;

/// @brief View of buffer of bytes.
struct ByteView
{
     std::byte *data{};  ///< pointer to buffer data.
     std::size_t size{}; ///< size of buffer, in bytes.

     /// @brief Bool conversion operator.
     constexpr explicit operator bool() const noexcept
     {
          return data && size;
     }
};


/// @brief View of immutable buffer of bytes.
struct ConstByteView
{
     const std::byte *data{}; ///< pointer to buffer data.
     std::size_t size{};      ///< size of buffer, in bytes.

     /// @brief Bool conversion operator.
     constexpr explicit operator bool() const noexcept
     {
          return data && size;
     }
};


/// @brief Data of the asset.
struct AssetData
{
     ContentTypeId type_id{}; ///< identifier of asset data type.
     ConstByteView data{};    ///< binary buffer with asset data.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_DEFS_H
