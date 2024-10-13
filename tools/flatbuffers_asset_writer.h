/// @file
/// @brief File with definition of FlatBuffersAssetWriter class.
#ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_WRITER_H
#define _16NAR_TOOLS_FLATBUFFERS_ASSET_WRITER_H

#include <iasset_writer.h>

namespace _16nar::tools
{

/// @brief Class for writing asset data in FlatBuffers format.
class ENGINE_API FlatBuffersAssetWriter : public IAssetWriter
{
public:
     /// @brief IAssetWriter::write_asset(const ResourceData&, const std::string&)
     virtual void write_asset( const ResourceData& resource, const std::string& dir ) override;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_WRITER_H
