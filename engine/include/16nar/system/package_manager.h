/// @file
/// @brief Header file with PackageManager class definition.
#ifndef _16NAR_PACKAGE_MANAGER_H
#define _16NAR_PACKAGE_MANAGER_H

#include <string_view>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <16nar/16nardefs.h>

namespace _16nar
{

class IRenderApi;

/// @brief Class for control over resource packages.
class PackageManager
{
public:
     PackageManager() = default;

     /// @brief Load resource package and create all its resources.
     /// @details Package can be packed into single file with .nrs extension.
     /// If package is unpacked, then resources should be in directory with same name.
     /// @param[in] filename path to resource package file or directory.
     /// @param[in] unpacked true if package compiled into .nrs file, false otherwise.
     /// @return true if package is loaded successfully, false otherwise.
     bool load_package( std::string_view name, bool unpacked = false );

     /// @brief Unload resource package and all its resources.
     /// @param[in] filename path to resource package file.
     void unload_package( std::string_view name );

     /// @brief Check if given package is loaded.
     /// @param[in] name name of package file.
     /// @return true if package is loaded, false otherwise.
     bool is_package_loaded( std::string_view name ) const;

     /// @brief Check if given resource is loaded.
     /// @param[in] name name of resource.
     /// @return true if resource is loaded, false otherwise.
     bool is_resource_loaded( std::string_view name ) const;

     /// @brief Set directory containing resource packages.
     /// @param[in] dirname path to directory.
     void set_package_dir( std::string_view dirname );

     /// @brief Get loaded resource.
     /// @param[in] name name of resource.
     /// @return valid resource if it exists, empty resource otherwise.
     Resource get_resource( std::string_view name ) const;

     /// @brief Release all saved information.
     void clear();

private:
     /// @brief Map of resource names and resource handlers (names of form "package_name/resource_name").
     using ResourceMap = std::map< std::string, Resource >;

     /// @brief Map of resource handlers and iterators to their names.
     using NameMap = std::unordered_map< Resource, ResourceMap::const_iterator >;

     /// @brief Load unpacked resource package from directory.
     /// @details This function scans directory and load resources
     /// from files in it. It is intended for usage in debug builds.
     /// @param[in] filename path to resource package directory.
     /// @return true if package is loaded successfully, false otherwise.
     bool load_unpacked( std::string_view dirname );

private:
     ResourceMap resources_;                      ///< all currently loaded resources.
     NameMap names_;                              ///< names of all currently loaded resources.
     std::unordered_set< std::string > packages_; ///< all currently loaded packages.
     std::string pkg_dir_;                        ///< path to directory containig resource packages.
};

} // namespace _16nar

#endif // #ifndef _16NAR_PACKAGE_MANAGER_H
