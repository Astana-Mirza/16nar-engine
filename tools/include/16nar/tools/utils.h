/// @file
/// @brief Utility functions for reading and writing asset data.
#ifndef _16NAR_TOOLS_UTILS_H
#define _16NAR_TOOLS_UTILS_H

#include <16nar/render/render_defs.h>

#include <16nar/tools/iasset_reader.h>
#include <16nar/tools/iasset_writer.h>

#include <string>

namespace _16nar::tools
{

/// @brief Read raw data from file.
/// @param[in] path path to file.
/// @param[out] data_size size of read data.
/// @param[in] zero_terminate does the data need to be zero terminated.
/// @throws std::runtime_error if file does not exist.
/// @return pointer to read data.
ENGINE_API DataSharedPtr read_binary( const std::string& path,
     std::size_t& data_size, bool zero_terminate = false );

/// @brief Read raw data from file.
/// @param[in] path path to file.
/// @param[in] data data to be written.
/// @param[in] zero_terminated is the data zero terminated (in this case, last zero byte will not be written).
/// @throws std::runtime_error if unable to open file.
/// @param[in] data_size size of read data.
ENGINE_API void write_binary( const std::string& path,
     DataSharedPtr data, std::size_t data_size, bool zero_terminated = false );

/// @brief Get correct filename.
/// @details If base directory is not empty and path is relative, then prepend base directory to path. 
/// @param[in] base_dir base directory to be prepended to file path.
/// @param[in] path path to the file.
/// @return final filename to be used for file operations.
ENGINE_API std::string correct_path( const std::string& base_dir, const std::string& path );

/// @brief Get channel count used for the data format.
/// @param[in] format data format.
/// @throws std::runtime_error if format cannot be saved or loaded.
/// @return channel count used for the data format.
ENGINE_API int get_channel_count( BufferDataFormat format );

/// @brief Create asset reader of given format.
/// @param[in] in_dir directory with input data, if needed by format.
/// @param[in] format format of asset reader.
/// @return asset reader.
ENGINE_API std::unique_ptr< IAssetReader > create_asset_reader( const std::string& in_dir,
     PackageFormat format );

/// @brief Create asset writer of given format.
/// @param[in] out_dir directory for output data, if needed by format.
/// @param[in] format format of asset writer.
/// @return asset writer.
ENGINE_API std::unique_ptr< IAssetWriter > create_asset_writer( const std::string& out_dir,
     PackageFormat format );

} // namespace _16nar::tools

#endif // _16NAR_TOOLS_BINARY_UTILS_H
