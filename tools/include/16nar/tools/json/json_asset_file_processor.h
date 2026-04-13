/// @file
/// @brief File with JsonAssetFileProcessor class definition.
#ifndef _16NAR_TOOLS_JSON_ASSET_FILE_PROCESSOR_H
#define _16NAR_TOOLS_JSON_ASSET_FILE_PROCESSOR_H

#include <16nar/tools/iasset_file_processor.h>

namespace _16nar::tools
{

/// @brief Class for processing data asset file in JSON format.
class ENGINE_API JsonAssetFileProcessor : public IAssetFileProcessor
{
public:
     /// @brief Constructor.
     /// @param[in] memory_resource memory resource for data allocations.
     explicit JsonAssetFileProcessor( std::pmr::memory_resource& memory_resource ) noexcept;

     /// @copydoc IAssetFileProcessor::read_asset_data(const File&)
     SharedBufferPtr read_asset_data( const File& file ) override;

     /// @copydoc IAssetFileProcessor::write_asset_data(ConstByteView, File&)
     bool write_asset_data( ConstByteView buffer, File& file ) override;

     /// @copydoc IAssetFileProcessor::make_asset_reader(ConstByteView)
     IAssetReaderPtr make_asset_reader( ConstByteView buffer ) override;

     /// @copydoc IAssetFileProcessor::make_asset_writer()
     IAssetWriterPtr make_asset_writer() override;

private:
     std::pmr::memory_resource& memory_resource_; ///< memory resource for data allocations.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_JSON_ASSET_FILE_PROCESSOR_H
