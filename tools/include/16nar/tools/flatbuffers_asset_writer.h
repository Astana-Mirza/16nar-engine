/// @file
/// @brief File with definition of FlatBuffersAssetWriter class.
#ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_WRITER_H
#define _16NAR_TOOLS_FLATBUFFERS_ASSET_WRITER_H

#include <16nar/tools/iasset_writer.h>

namespace _16nar::tools
{

/// @brief Class for writing asset data in FlatBuffers format.
class ENGINE_API FlatBuffersAssetWriter final : public IAssetWriter
{
public:
     /// @copydoc IFileOperator::get_file_ext() const noexcept
     std::string get_file_ext() const noexcept override;

     /// @copydoc IFileOperator::get_pkg_ext() const noexcept
     std::string get_pkg_ext() const noexcept override;

     /// @copydoc IAssetWriter::write_asset(std::ostream&, const ResourceData&)
     void write_asset( std::ostream& output, const ResourceData& resource ) override;

     /// @copydoc IAssetWriter::write_package(std::ostream&, const PackageData&)
     void write_package( std::ostream& output, const PackageData& package ) override;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_WRITER_H
