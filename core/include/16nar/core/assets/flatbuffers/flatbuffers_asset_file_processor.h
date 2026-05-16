/// @file
/// @brief File with FlatBuffersAssetFileProcessor class definition.
#ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_FILE_PROCESSOR_H
#define _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_FILE_PROCESSOR_H

#include <16nar/core/assets/iasset_file_processor.h>

namespace _16nar::assets
{

/// @brief Class for processing data asset file in flatbuffers format.
class NARENGINE_ASSETS_FB_API FlatBuffersAssetFileProcessor : public IAssetFileProcessor
{
public:
     /// @brief Constructor.
     /// @param[in] memory_resource memory resource for data allocations.
     /// @param[in] initial_size initial size of builder buffer for asset writer.
     /// @param[in] check_names true if, for asset writer, name uniqueness needs to be
     /// checked while saving children sets (has performance and memory costs), false otherwise.
     FlatBuffersAssetFileProcessor(
          std::pmr::memory_resource& memory_resource,
          std::size_t initial_size = 1024, bool check_names = false ) noexcept;

     /// @copydoc IAssetFileProcessor::read_asset_data(const system::File&)
     memory::SharedBufferPtr read_asset_data( const system::File& file ) override;

     /// @copydoc IAssetFileProcessor::write_asset_data(memory::ConstByteView, system::File&)
     bool write_asset_data( memory::ConstByteView buffer, system::File& file ) override;

     /// @copydoc IAssetFileProcessor::make_asset_reader()
     IAssetReaderPtr make_asset_reader() override;

     /// @copydoc IAssetFileProcessor::make_asset_writer()
     IAssetWriterPtr make_asset_writer() override;

private:
     std::pmr::memory_resource& memory_resource_; ///< memory resource for data allocations.
     std::size_t initial_size_;                   ///< initial size of builder buffer for asset writer.
     bool check_names_;                           ///< check names of children in sets for asset writer.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_FILE_PROCESSOR_H
