/// @file
/// @brief File with FlatBuffersAssetFileProcessor class definition.
#ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_FILE_PROCESSOR_H
#define _16NAR_TOOLS_FLATBUFFERS_ASSET_FILE_PROCESSOR_H

#include <16nar/tools/iasset_file_processor.h>

namespace _16nar::tools
{

/// @brief Class for processing data asset file in flatbuffers format.
class ENGINE_API FlatBuffersAssetFileProcessor : public IAssetFileProcessor
{
public:
     /// @brief Constructor.
     /// @param[in] memory_resource memory resource for data allocations.
     explicit FlatBuffersAssetFileProcessor( std::pmr::memory_resource& memory_resource ) noexcept;

     /// @copydoc IAssetFileProcessor::read_asset_data(const File&)
     SharedBufferPtr read_asset_data( const File& file ) override;

     /// @copydoc IAssetFileProcessor::write_asset_data(ConstByteView, File&)
     bool write_asset_data( ConstByteView buffer, File& file ) override;

     /// @copydoc IAssetFileProcessor::make_asset_reader(ConstByteView)
     IAssetReaderPtr make_asset_reader( ConstByteView buffer ) override;

private:
     std::pmr::memory_resource& memory_resource_; ///< memory resource for data allocations.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_FLATBUFFERS_ASSET_FILE_PROCESSOR_H
