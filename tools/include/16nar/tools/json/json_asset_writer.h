/// @file
/// @brief File with JsonAssetWriter class definition.
#ifndef _16NAR_TOOLS_JSON_ASSET_WRITER_H
#define _16NAR_TOOLS_JSON_ASSET_WRITER_H

#include <16nar/tools/iasset_writer.h>

#include <nlohmann/json.hpp>

#include <string>

namespace _16nar::tools
{

/// @brief Class for reading data asset in JSON format.
class ENGINE_API JsonAssetWriter : public IAssetWriter
{
public:
     /// @brief Default constructor.
     JsonAssetWriter();

     /// @copydoc IAssetWriter::write_asset(std::string_view, AssetData, const std::vector<std::uint32_t>&, bool)
     std::uint32_t write_asset( std::string_view name, AssetData content,
          const std::vector< std::uint32_t >& children = {}, bool is_array = false ) override;

     /// @copydoc IAssetWriter::finish(std::uint32_t)
     ConstByteView finish( std::uint32_t root_id ) override;

     /// @copydoc IAssetWriter::reset()
     void reset() override;

private:
     std::unordered_map< std::uint32_t, nlohmann::json > ids_;   ///< identifiers of written assets.
     std::string result_;                                        ///< resulting JSON.
     std::uint32_t current_id_;                                  ///< current asset id.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_JSON_ASSET_WRITER_H
