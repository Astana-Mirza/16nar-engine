/// @file
/// @brief File with JsonAssetFileProcessor class definition.
#ifndef _16NAR_CORE_ASSETS_JSON_ASSET_FILE_PROCESSOR_H
#define _16NAR_CORE_ASSETS_JSON_ASSET_FILE_PROCESSOR_H

#include <16nar/core/assets/iasset_file_processor.h>

namespace _16nar::assets
{

/// @brief Class for processing data asset file in JSON format.
class NARENGINE_ASSETS_JSON_API JsonAssetFileProcessor : public IAssetFileProcessor
{
public:
     /// @brief Constructor.
     /// @param[in] resource memory resource for utiliy data allocations.
     /// @param[in] big_resource memory resource for payload allocations.
     explicit JsonAssetFileProcessor( std::pmr::memory_resource& resource,
          std::pmr::memory_resource& big_resource ) noexcept;

     /// @copydoc IAssetFileProcessor::read_asset_data(const system::File&)
     memory::SharedBufferPtr read_asset_data( const system::File& file ) override;

     /// @copydoc IAssetFileProcessor::write_asset_data(memory::ConstByteView, system::File&)
     bool write_asset_data( memory::ConstByteView buffer, system::File& file ) override;

     /// @copydoc IAssetFileProcessor::make_asset_reader()
     IAssetReaderPtr make_asset_reader() override;

     /// @copydoc IAssetFileProcessor::make_asset_writer()
     IAssetWriterPtr make_asset_writer() override;

private:
     std::pmr::memory_resource& resource_;        ///< memory resource for utiliy data allocations.
     std::pmr::memory_resource& big_resource_;    ///< memory resource for payload data allocations.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_JSON_ASSET_FILE_PROCESSOR_H
