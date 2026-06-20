/// @file
/// @brief File with UnifiedStorage class definition.
#ifndef _16NAR_CORE_RESOURCES_UNIFIED_STORAGE_H
#define _16NAR_CORE_RESOURCES_UNIFIED_STORAGE_H

#include <16nar/core/assets/defs.h>

#include <16nar/platform/system/file.h>
#include <16nar/platform/strings/static_name.h>
#include <16nar/platform/strings/name_manager.h>
#include <16nar/platform/memory/shared_buffer_ptr.h>

#include <unordered_map>
#include <filesystem>

namespace _16nar::resources
{

/// @brief Storage of all application files and resource data.
class NARENGINE_CORE_API UnifiedStorage
{
public:
     /// @brief Get corrected path.
     /// @details If path is relative, then prepend base directory to it. 
     /// @param[in] base_dir base directory to be prepended to file path.
     /// @param[in] path path to the file.
     /// @return final path to be used for file operations.
     static std::filesystem::path correct_path(
          const std::filesystem::path& base_dir,
          const std::filesystem::path& path );

     /// @brief Constructor.
     /// @throws std::runtime_error if @b name_table or @b file_processor is null
     /// or no suitable memory domain ("asset") present in @b memory_manager.
     /// @param[in] base_dir base directory of the application.
     /// @param[in] memory_manager memory manager for allocations.
     /// @param[in] name_table name table.
     /// @param[in] file_processor asset file processor.
     UnifiedStorage(
          const std::filesystem::path& base_dir,
          memory::MemoryManager& memory_manager,
          strings::NameManager& name_manager,
          assets::IAssetFileProcessorPtr file_processor );

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
     std::filesystem::path get_path( strings::StaticName name ) const;

     /// @brief Load the resource data by name.
     /// @param[in] name name of the resource.
     /// @return data of the resource, nullptr if resource with given @b name does not exist.
     memory::SharedBufferPtr load( strings::StaticName name );

     /// @brief Mount the package.
     /// @details If there are resources with the same name in differrent packages, then
     /// the latest loaded resource (i.e. from the latest mounted package) will be loaded
     /// at next load request. Patching may be implemented with this feature.
     /// @param[in] path path to the package (not the database) relative to the base directory.
     /// @return true on success, false otherwise.
     bool mount( const std::filesystem::path& path );

     /// @brief Set asset data convertor.
     /// @param[in] convertor asset data convertor.
     void set_convertor( assets::IAssetDataConvertorPtr convertor ) noexcept;

     /// @brief Get asset file processor.
     /// @return asset file processor.
     assets::IAssetFileProcessorPtr get_file_processor() const noexcept;

     /// @brief Get asset data convertor.
     /// @return asset data convertor.
     assets::IAssetDataConvertorPtr get_convertor() const noexcept;

private:
     UnifiedStorage( const UnifiedStorage& ) = delete;
     UnifiedStorage& operator=( const UnifiedStorage& ) = delete;

private:
     /// @brief Description of resource in the package.
     struct ResourceDesc
     {
          strings::StaticName type_id{};     ///< type name of the resource.
          std::uint32_t chunk_id{};          ///< id of starting chunk of the resource.
          std::uint32_t size{};              ///< size of the resource in the package, in bytes.
          std::uint32_t orig_size{};         ///< original size of the resource data, in bytes.
          strings::StaticName package{};     ///< name of the package in which the resource resides.
     };

     /// @brief Package containing resources.
     struct ResourcePackage
     {
          /// @todo add compression information
          std::uint32_t chunk_size{};   ///< size of one package chunk, in bytes.
          system::File database{};      ///< file of the package database.
     };

     using ResourceMap = std::unordered_map< strings::StaticName, ResourceDesc >;
     using ResourcePackageMap = std::unordered_map< strings::StaticName, ResourcePackage >;
     friend class ResourceParser;

     ResourceMap resources_;                           ///< resources with their names.
     ResourcePackageMap packages_;                     ///< resource packages with theris names.
     std::filesystem::path base_dir_;                  ///< base directory of the application.
     strings::NameTable *name_table_;                  ///< name table.
     assets::IAssetFileProcessorPtr file_processor_;   ///< asset file processor.
     assets::IAssetReaderPtr asset_reader_;            ///< asset reader.
     assets::IAssetDataConvertorPtr convertor_;        ///< asset data convertor.
     std::pmr::memory_resource *memory_;               ///< memory resource for asset reading allocations.
     bool unpacked_;                                   ///< read unpacked resources insted of packages.
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_UNIFIED_STORAGE_H
