/// @file
/// @brief File with IAssetWriter interface definition.
#ifndef _16NAR_TOOLS_IASSET_WRITER_H
#define _16NAR_TOOLS_IASSET_WRITER_H

#include <16nar/tools/defs.h>

#include <vector>
#include <string_view>

namespace _16nar::tools
{

/// @brief Interface for writing data asset.
/// @details All assets written by a single writer are stored in one buffer. The assets
/// are stored in a tree. Non-zero unsigned numbers are used for identification of written assets.
class ENGINE_API IAssetWriter
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetWriter() = default;

     /// @brief Write asset to the buffer.
     /// @details Every asset has an identifier which can be used only once, so
     /// assets can be structured into a tree.
     /// @param[in] name name of the asset, may be empty.
     /// @param[in] content content of the asset, may be empty.
     /// @param[in] children children asset identifiers, may be empty.
     /// @param[in] is_array true if asset's children are stored in array rather than map, false otherwise.
     /// @return identifier of written asset, 0 in case of error.
     virtual std::uint32_t write_asset( std::string_view name, AssetData content,
          const std::vector< std::uint32_t >& children = {}, bool is_array = false ) = 0;

     /// @brief Finish the write and get the result buffer.
     /// @details After the buffer is accessed, further write is impossible.
     /// By the time this function is called, the @b root_id must remain the only unused asset identifier.
     /// @param[in] root_id identifier of the tree root asset.
     /// @return result buffer.
     virtual ConstByteView finish( std::uint32_t root_id ) = 0;

     /// @brief Reset current state to default.
     virtual void reset() = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_WRITER_H
