/// @file
/// @brief File with definition of data structures of resources.
#ifndef _16NAR_TOOLS_RESOURCE_PACKAGE_H
#define _16NAR_TOOLS_RESOURCE_PACKAGE_H

#include <16nar/16nardefs.h>

#include <string>
#include <vector>
#include <any>

namespace _16nar::tools
{

/// @brief Format of resource package or asset file.
enum class PackageFormat
{
     Json,          ///< JSON.
     FlatBuffers    ///< FlatBuffers buffer.
};


/// @brief Data of resource used in engine.
struct ResourceData
{
     std::any params;                        ///< resource load parameters.
     std::string name;                       ///< name of resource.
     std::vector< uint32_t > data_sizes;     ///< sizes of resource data objects.
     ResourceType type;                      ///< type of resource.
};


/// @brief Data of resource package used in engine.
struct PackageData
{
     std::vector< ResourceData > resources;  ///< resources of the package.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_RESOURCE_PACKAGE_H
