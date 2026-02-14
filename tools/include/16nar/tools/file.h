/// @file
/// @brief Utility functions for reading and writing asset files.
#ifndef _16NAR_TOOLS_FILE_H
#define _16NAR_TOOLS_FILE_H

#include <16nar/tools/defs.h>

#include <filesystem>
#include <cstdio>

namespace _16nar::tools
{

/// @brief Class representing an open file.
class ENGINE_API File
{
public:
     /// @brief Origin of file pointer modification.
     enum SeekOrigin
     {
          Set = SEEK_SET,     ///< beginning of the file.
          Cur = SEEK_CUR,     ///< current file pointer.
          End = SEEK_END      ///< end of the file.
     };

     /// @brief Default constructor.
     File() noexcept;

     /// @brief Move constructor.
     /// @param[in] other object to be moved.
     File( File&& other ) noexcept;

     /// @brief Move assignment operator.
     /// @param[in] rhs right operand.
     /// @return Current object.
     File& operator=( File&& rhs ) noexcept;

     /// @brief Destructor, closes the file.
     ~File();

     /// @brief Open existing file with given path.
     /// @details When the file is being opened for reading, it must exist (false will be returned otherwise).
     /// When the file is being opened for writing, it will be created or truncated, depending on its existence.
     /// If the file is already opened, it will be closed first.
     /// @param[in] path path of file to open.
     /// @param[in] write true - open file for writing, false - open file for reading.
     /// @return true on success, false otherwise.
     bool open( const std::filesystem::path& path, bool write = false ) noexcept;

     /// @brief Check if the file is open.
     /// @return true if the file is open, false otherwise.
     bool is_open() const noexcept;

     /// @brief Close the file.
     void close() noexcept;

     /// @brief Get current pointer of file.
     /// @return current pointer of file, static_cast< std::size_t >( -1L ) on error.
     std::size_t tell() const noexcept;

     /// @brief Set file pointer to specified offset.
     /// @param[in] offset desired offset.
     /// @param[in] origin origin of pointer modification.
     /// @return true on success, false on failure.
     bool seek( std::size_t offset, SeekOrigin origin ) noexcept;

     /// @brief Write buffer to the file.
     /// @param[in] buffer buffer to be written.
     /// @return count of written bytes.
     std::size_t write( ConstByteView buffer ) noexcept;

     /// @brief Read buffer from the file.
     /// @param[out] buffer buffer to be filled with read bytes.
     /// @return count of read bytes.
     std::size_t read( ByteView buffer ) const noexcept;

     /// @brief Get file size.
     /// @details In case of error, the file's current position may be altered.
     /// @return file size or 0 in case of error.
     std::size_t get_size() const noexcept;

     /// @todo add memory mapping support

     /// @brief Map region of file into memory.
     /// @param region region of file to be mapped.
     /// @return memory mapping of selected region.
     // virtual MemoryMapping map_to_memory( ByteView region ) = 0;

private:
     FILE *handle_;     ///< file handle.
};


/// @brief Get correct filename.
/// @details If base directory is not empty and path is relative, then prepend base directory to path. 
/// @param[in] base_dir base directory to be prepended to file path.
/// @param[in] path path to the file.
/// @return final filename to be used for file operations.
ENGINE_API std::filesystem::path correct_path(
     const std::filesystem::path& base_dir,
     const std::filesystem::path& path );

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_FILE_H
