/// @file
/// @brief File with IAssetFileProcessor interface definition.
#ifndef _16NAR_TOOLS_IASSET_FILE_PROCESSOR_H
#define _16NAR_TOOLS_IASSET_FILE_PROCESSOR_H

#include <16nar/tools/defs.h>
#include <16nar/tools/misc/file.h>
#include <16nar/tools/misc/shared_buffer_ptr.h>

namespace _16nar::tools
{

/// @brief Interface for processing data asset file.
class ENGINE_API IAssetFileProcessor
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetFileProcessor() = default;

     /// @brief Read asset data from input file.
     /// @details The data is read from file and stored in format which is valid for
     /// creation of asset reader which reads the data.
     /// @param[in] file input file.
     /// @return asset data buffer.
     virtual SharedBufferPtr read_asset_data( const File& file ) = 0;

     /// @brief Write asset data to output file.
     /// @param[in] buffer asset data buffer.
     /// @param[out] file output file.
     /// @return true on success, false otherwise.
     virtual bool write_asset_data( ConstByteView buffer, File& file ) = 0;

     /// @brief Make asset reader.
     /// @return asset reader.
     virtual IAssetReaderPtr make_asset_reader() = 0;

     /// @brief Make asset writer.
     /// @return asset writer.
     virtual IAssetWriterPtr make_asset_writer() = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_FILE_PROCESSOR_H
