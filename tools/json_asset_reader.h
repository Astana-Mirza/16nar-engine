/// @file
/// @brief File with definition of JsonAssetReader class.
#ifndef _16NAR_JSON_ASSET_READER_UTILS_H
#define _16NAR_JSON_ASSET_READER_UTILS_H

#include <iasset_reader.h>

namespace _16nar::tools
{

/// @brief Class for reading asset data in JSON format.
class ENGINE_API JsonAssetReader : public IAssetReader
{
public:
     /// @copydoc IAssetReader::read_asset(std::istream&, ResourceData&)
     virtual void read_asset( std::istream& file, ResourceData& resource ) override;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_JSON_ASSET_READER_UTILS_H
