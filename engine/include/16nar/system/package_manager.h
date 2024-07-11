/// @file
/// @brief Header file with PackageManager class definition.
#ifndef _16NAR_PACKAGE_MANAGER_H
#define _16NAR_PACKAGE_MANAGER_H

#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>

#include <16nar/16nardefs.h>

namespace _16nar
{

class IRenderApi;

/// @brief Class for control over resource packages.
class PackageManager
{
public:
     /// @brief Map of resource package names and named resource handlers.
     using PackageMap = std::unordered_map< std::string, std::unordered_map< std::string, Resource > >;

     /// @brief Constructor.
     /// @param[in] render_api API for graphic resource management.
     PackageManager( IRenderApi& render_api );

     /// @brief Load resource package and create all its resources.
     /// @details Package can be packed into single file with .nrs extension.
     /// If package is unpacked, then resources should be in directory with same name.
     /// @param[in] filename path to resource package file or directory.
     /// @param[in] unpacked true if package compiled into .nrs file, false otherwise.
     /// @return true if package is loaded successfully, false otherwise.
     bool load_package( std::string_view filename, bool unpacked = false );

     /// @brief Unload resource package and all its resources.
     /// @param[in] filename path to resource package file.
     void unload_package( std::string_view filename );

     /// @brief Check if given package is loaded.
     /// @param[in] filename name of package file.
     /// @return true if package is loaded, false otherwise.
     bool is_loaded( std::string_view package ) const;

     /// @brief Check if given resource is loaded.
     /// @param[in] package name of package file.
     /// @param[in] resource name of resource.
     /// @return true if package is loaded, false otherwise.
     bool is_resource_loaded( std::string_view package, std::string_view resource ) const;

     /// @brief Set directory containing resource packages.
     /// @param[in] dirname path to directory.
     void set_package_dir( std::string_view dirname );

     /// @brief Get loaded resource or empty if it is not found.
     /// @param[in] package name of package file.
     /// @param[in] resource name of resource.
     Resource get_resource( std::string_view package, std::string_view resource ) const;

private:
     /// @brief Load unpacked resource package from directory.
     /// @details This function scans directory and load resources
     /// from files in it. It is intended for usage in debug builds.
     /// @param[in] filename path to resource package directory.
     /// @return true if package is loaded successfully, false otherwise.
     bool load_unpacked( std::string_view dirname );

private:
     PackageMap packages_;    ///< all currently loaded resources.
     std::string pkg_dir_;    ///< path to directory containig resource packages.
     IRenderApi& render_api_; ///< API for graphic resource management.
};

} // namespace _16nar

#endif // #ifndef _16NAR_PACKAGE_MANAGER_H
