/// @file
/// @brief Header file with PackageManager class definition.
#ifndef _16NAR_PACKAGE_MANAGER_H
#define _16NAR_PACKAGE_MANAGER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/iasset_reader.h>

#include <string_view>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>

namespace _16nar
{

class IRenderApi;

/// @brief Class for control over resource packages.
/// @details Game object must contain render API in order to use this class.
/// Render API is used to load grapthical resources. If it is not set, the
/// application will fail.
class ENGINE_API PackageManager
{
public:
     /// @brief Constructor.
     /// @param[in] reader asset reader.
     PackageManager( std::unique_ptr< tools::IAssetReader >&& reader );

     /// @brief Destructor, unloads all packages.
     ~PackageManager();

     /// @brief Load resource package and create all its resources.
     /// @details Package can be packed into single file.
     /// If package is unpacked, then resources should be in directory with same name.
     /// If load of a resource from a package fails, then all previously loaded resources
     /// from this package should be unloaded. If this unload fails (exception is thrown
     /// by render API), it will not be caught here, because it means something bad happened.
     /// @param[in] filename path to resource package file or directory.
     /// @return true if package is loaded successfully, false otherwise.
     bool load_package( const std::string& name );

     /// @brief Unload resource package and all its resources.
     /// @details Unload may throw, exception is not caught in this function.
     /// @param[in] filename path to resource package file.
     void unload_package( const std::string& name );

     /// @brief Check if given package is loaded.
     /// @param[in] name name of package file.
     /// @return true if package is loaded, false otherwise.
     bool is_package_loaded( const std::string& name ) const;

     /// @brief Check if given resource is loaded.
     /// @param[in] name name of resource.
     /// @return true if resource is loaded, false otherwise.
     bool is_resource_loaded( const std::string& name ) const;

     /// @brief Set unpacked mode of package loading.
     /// @param[in] mode true if packages' resources stored in separate files, false otherwise.
     void set_unpacked_mode( bool mode ) noexcept;

     /// @brief Set directory containing resource packages.
     /// @param[in] dirname path to directory.
     /// @throws std::bad_alloc.
     void set_package_dir( const std::string& dirname );

     /// @brief Get loaded resource.
     /// @param[in] name name of resource.
     /// @return valid resource if it exists, empty resource otherwise.
     Resource get_resource( const std::string& name ) const;

     /// @brief Release all saved information.
     /// @details Unload may throw, exception is not caught in this function.
     void clear();

private:
     /// @brief Pointer to abstract asset reader.
     using AssetReaderPtr = std::unique_ptr< tools::IAssetReader >;

     /// @brief Map of resource names and resource handlers (names of form "package_name/resource_name").
     using ResourceMap = std::map< std::string, Resource >;

     /// @brief Map of resource handlers and iterators to their names.
     using NameMap = std::unordered_map< Resource, ResourceMap::const_iterator >;

     /// @brief Load unpacked resource package from directory.
     /// @details This function scans directory and load resources
     /// from files in it. It is intended for usage in debug builds.
     /// @param[in] filename path to resource package directory.
     /// @return true if package is loaded successfully, false otherwise.
     bool load_unpacked( const std::string& dirname );

private:
     ResourceMap resources_;                      ///< all currently loaded resources.
     NameMap names_;                              ///< names of all currently loaded resources.
     std::unordered_set< std::string > packages_; ///< all currently loaded packages.
     std::string pkg_dir_;                        ///< path to directory containig resource packages.
     AssetReaderPtr reader_;                      ///< asset reaer.
     bool unpacked_mode_;                         ///< are packages' resources stored in separate files (false by default).
};

} // namespace _16nar

#endif // #ifndef _16NAR_PACKAGE_MANAGER_H
