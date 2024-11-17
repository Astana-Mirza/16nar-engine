/// @file
/// @brief File with definition of JsonAssetReader class.
#ifndef _16NAR_TOOLS_JSON_ASSET_READER_H
#define _16NAR_TOOLS_JSON_ASSET_READER_H

#include <16nar/tools/iasset_reader.h>
#include <16nar/tools/json_file_operator.h>

namespace _16nar::tools
{

/// @brief Class for reading asset data in JSON format.
class ENGINE_API JsonAssetReader final : public JsonFileOperator< IAssetReader >
{
public:
     /// @brief Contructor.
     /// @param in_dir directory for reading raw files of assets, if empty, then use current directory.
     JsonAssetReader( const std::string& in_dir = std::string{} );

     /// @copydoc IAssetReader::read_asset(std::istream&)
     ResourceData read_asset( std::istream& file ) override;

     /// @copydoc IAssetReader::read_package(std::istream&)
     PackageData read_package( std::istream& input ) override;

private:
     std::string in_dir_;    ///< directory for reading raw files of assets.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_JSON_ASSET_READER_H
