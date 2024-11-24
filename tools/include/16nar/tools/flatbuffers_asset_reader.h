/// @file
/// @brief File with definition of FlatBuffersAssetReader class.
#ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_READER_H
#define _16NAR_TOOLS_FLATBUFFERS_ASSET_READER_H

#include <16nar/tools/iasset_reader.h>

namespace _16nar::tools
{

/// @brief Class for reading asset data in flatbuffers format.
class ENGINE_API FlatBuffersAssetReader final : public IAssetReader
{
public:
     /// @copydoc IFileOperator::get_file_ext() const noexcept
     const char *get_file_ext() const noexcept override;

     /// @copydoc IFileOperator::get_pkg_ext() const noexcept
     const char *get_pkg_ext() const noexcept override;

     /// @copydoc IAssetReader::read_asset(std::istream&)
     ResourceData read_asset( std::istream& input ) override;

     /// @copydoc IAssetReader::read_package(std::istream&)
     PackageData read_package( std::istream& input ) override;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_READER_H
