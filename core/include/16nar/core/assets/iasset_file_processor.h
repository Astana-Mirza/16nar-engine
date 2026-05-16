/// @file
/// @brief File with IAssetFileProcessor interface definition.
#ifndef _16NAR_CORE_ASSETS_IASSET_FILE_PROCESSOR_H
#define _16NAR_CORE_ASSETS_IASSET_FILE_PROCESSOR_H

#include <16nar/core/assets/defs.h>

#include <16nar/platform/system/file.h>
#include <16nar/platform/memory/shared_buffer_ptr.h>

namespace _16nar::assets
{

/// @brief Interface for processing data asset file.
class NARENGINE_CORE_API IAssetFileProcessor
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetFileProcessor() = default;

     /// @brief Read asset data from input file.
     /// @details The data is read from file and stored in format which is valid for
     /// creation of asset reader which reads the data.
     /// @param[in] file input file.
     /// @return asset data buffer.
     virtual memory::SharedBufferPtr read_asset_data( const system::File& file ) = 0;

     /// @brief Write asset data to output file.
     /// @param[in] buffer asset data buffer.
     /// @param[out] file output file.
     /// @return true on success, false otherwise.
     virtual bool write_asset_data( memory::ConstByteView buffer, system::File& file ) = 0;

     /// @brief Make asset reader.
     /// @return asset reader.
     virtual IAssetReaderPtr make_asset_reader() = 0;

     /// @brief Make asset writer.
     /// @return asset writer.
     virtual IAssetWriterPtr make_asset_writer() = 0;
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_IASSET_FILE_PROCESSOR_H
