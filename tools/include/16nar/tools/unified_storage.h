/// @file
/// @brief File with UnifiedStorage class definition.
#ifndef _16NAR_TOOLS_UNIFIED_STORAGE_H
#define _16NAR_TOOLS_UNIFIED_STORAGE_H

#include <16nar/tools/defs.h>

#include <16nar/tools/file.h>
#include <16nar/tools/static_name.h>
#include <16nar/tools/shared_buffer_ptr.h>

#include <unordered_map>
#include <filesystem>

namespace _16nar::tools
{

/// @brief Storage of all application files and data.
class ENGINE_API UnifiedStorage
{
public:
     /// @brief Constructor.
     /// @throws std::runtime_error if @b name_table or @b file_processor is null
     /// or no suitable allocator present in @b memory_manager.
     /// @param[in] base_dir base directory of the application.
     /// @param[in] memory_manager memory manager for allocations.
     /// @param[in] name_table name table.
     /// @param[in] file_processor asset file processor.
     UnifiedStorage(
          const std::filesystem::path& base_dir,
          MemoryManager& memory_manager,
          NameTablePtr name_table,
          IAssetFileProcessorPtr file_processor );

     /// @brief Set unpacked operation mode for the storage.
     /// @details While in unpacked mode, unified storage reads
     /// assets as individual files instead of packages.
     /// @param[in] mode unpacked mode flag.
     void set_unpacked_mode( bool mode ) noexcept;

     /// @brief Get unpacked mode flag.
     /// @return unpacked mode flag.
     bool get_unpacked_mode() const noexcept;

     /// @brief Get base directory of the application.
     /// @return base directory of the application.
     const std::filesystem::path& get_base_dir() const noexcept;

     /// @brief Get filesystem path by name.
     /// @param[in] name name of a file.
     /// @return path of the requested file or empty path if the @b name is unknown.
     std::filesystem::path get_path( StaticName name ) const;

     /// @brief Load the resource data by name.
     /// @param[in] name name of the resource.
     /// @return Data of the resource, nullptr if resource with given @b name does not exist.
     SharedBufferPtr load( StaticName name ) const;

     /// @brief Mount the package.
     /// @details If there are resources with the same name in differrent packages, then
     /// the latest loaded resource (i.e. from the latest mounted package) will be loaded
     /// at next load request. Patching may be implemented with this feature.
     /// @param[in] path path to the package (not the database) relative to the base directory.
     /// @return true on success, false otherwise.
     bool mount( const std::filesystem::path& path );

     /// @brief Set asset data convertor.
     /// @param[in] convertor asset data convertor.
     void set_convertor( IAssetDataConvertorPtr convertor ) noexcept;

     /// @brief Get asset file processor.
     /// @return asset file processor.
     IAssetFileProcessorPtr get_file_processor() const noexcept;

     /// @brief Get asset data convertor.
     /// @return asset data convertor.
     IAssetDataConvertorPtr get_convertor() const noexcept;

private:
     /// @brief Description of resource in the package.
     struct ResourceDesc
     {
          std::uint32_t chunk_id{};     ///< id of starting chunk of the resource.
          std::uint32_t size{};         ///< size of the resource in the package, in bytes.
          std::uint32_t orig_size{};    ///< original size of the resource data, in bytes.
          StaticName package{};         ///< name of the package in which the resource resides.
     };

     /// @brief Package containing resources.
     struct ResourcePackage
     {
          /// @todo add compression information
          std::uint32_t chunk_size{};   ///< size of one package chunk, in bytes.
          File database{};              ///< file of the package database.
     };

     using ResourceMap = std::unordered_map< StaticName, ResourceDesc >;
     using ResourcePackageMap = std::unordered_map< StaticName, ResourcePackage >;
     friend class ResourceParser;

     ResourceMap resources_;                 ///< resources with their names.
     ResourcePackageMap packages_;           ///< resource packages with theris names.
     std::filesystem::path base_dir_;        ///< base directory of the application.
     NameTablePtr name_table_;               ///< name table.
     IAssetFileProcessorPtr file_processor_; ///< asset file processor.
     IAssetDataConvertorPtr convertor_;      ///< asset data convertor.
     MemoryDomain *memory_;                  ///< memory domain for asset reading allocations.
     bool unpacked_;                         ///< read unpacked resources insted of packages.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_UNIFIED_STORAGE_H
