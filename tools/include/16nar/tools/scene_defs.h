/// @file
/// @brief File with definition of data structures of scene.
#ifndef _16NAR_TOOLS_SCENE_DEFS_H
#define _16NAR_TOOLS_SCENE_DEFS_H

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <memory>

#include <16nar/tools/iprops_reader.h>

namespace _16nar::tools
{

/// @brief Type of stored data for type-safe key-value pair usage.
enum class StoredDataType : uint8_t
{
     Unknown = 0,
     Uint64,
     Uint32,
     Uint16,
     Uint8,
     Int64,
     Int32,
     Int16,
     Int8,
     Bool,
     Float,
     Double,
     String,

     Uint64Arr,
     Uint32Arr,
     Uint16Arr,
     Uint8Arr,
     Int64Arr,
     Int32Arr,
     Int16Arr,
     Int8Arr,
     BoolArr,
     FloatArr,
     DoubleArr,
     StringArr,

     Vec2f,
     Vec3f,
     Vec4f,
     Vec2i,
     Vec3i,
     Vec4i,
     FloatRect,
     IntRect,
     ResourceIndex
};


/// @brief Item of data schema.
struct DataItem
{
     StoredDataType type = StoredDataType::Unknown;    ///< data type.
     bool mandatory{};                                 ///< necessity of presense in stored data.
};


/// @brief Schema of stored binary data.
struct DataSchema
{
     std::unordered_map< std::string, DataItem > items;     ///< data items.
     std::shared_ptr< IPropsReader > default_vals;          ///< default values reader.
};


/// @brief Dependencies from one package.
struct PackageDeps
{
     std::string name;                       ///< name of needed package.
     std::vector< std::string > elements;    ///< names of needed resources within package.
};


/// @brief Dependencies of one scene or scene piece file.
struct Dependencies
{
     std::vector< PackageDeps > resources;   ///< all needed resources.
     std::vector< PackageDeps > libs;        ///< all needed libraries.
     std::vector< PackageDeps > pieces;      ///< all needed scene pieces.
};


/// @brief Index of resource in scene's dependencies.
struct ResourceIndex
{
     uint16_t package{};      ///< index of package.
     uint16_t resource{};     ///< index of resource within package.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_SCENE_DEFS_H
