/// @file
/// @brief File with definition of JsonAssetWriter class.
#ifndef _16NAR_TOOLS_JSON_ASSET_WRITER_H
#define _16NAR_TOOLS_JSON_ASSET_WRITER_H

#include <16nar/tools/iasset_writer.h>
#include <16nar/tools/json_file_operator.h>

namespace _16nar::tools
{

/// @brief Class for writing asset data in JSON format.
/// @brief Assets are written into several files: main file with load parameters
/// and files with actual data. Data files may only be raw, which means they
/// contain only actual bytes of data without specific format (PNG, JPEG, etc).
class ENGINE_API JsonAssetWriter final : public JsonFileOperator< IAssetWriter >
{
public:
     /// @brief Contructor.
     /// @param out_dir directory for writing raw files of assets, if empty, then use current directory.
     JsonAssetWriter( const std::string& out_dir = std::string{} );

     /// @copydoc IAssetWriter::write_asset(std::ostream&, const ResourceData&)
     void write_asset( std::ostream& output, const ResourceData& resource ) override;

     /// @copydoc IAssetWriter::write_package(std::ostream&, const PackageData&)
     void write_package( std::ostream& output, const PackageData& package ) override;

private:
     std::string out_dir_;    ///< directory for writing raw files of assets.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_JSON_ASSET_WRITER_H
