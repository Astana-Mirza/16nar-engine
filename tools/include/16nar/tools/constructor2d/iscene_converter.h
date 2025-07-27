/// @file
/// @brief Header file with ISceneConverter interface definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_CONVERTER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_CONVERTER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/assets/iasset_reader.h>
#include <16nar/tools/constructor2d/iscene_reader.h>
#include <16nar/tools/constructor2d/inode_package_reader.h>

#include <filesystem>

namespace _16nar::tools::constructor2d
{

/// @brief Interface for converting scenes and node packages to specified format.
class ENGINE_API ISceneConverter
{
public:
     /// @brief Convert the scene and write result to file.
     /// @throws std::runtime_error in case of scene format errors.
     /// @param[in] scene_reader reader of the scene.
     /// @param[in] asset_reader reader of assets, required to read data schema.
     /// @param[in] resource_dir directory containing resources used by the scene.
     /// @param[in] out_file path to output file.
     virtual void convert_scene( ISceneReader& scene_reader, IAssetReader& asset_reader,
          const std::filesystem::path& resource_dir, const std::filesystem::path& out_file ) = 0;

     /// @brief Convert the node package and write result to file.
     /// @throws std::runtime_error in case of node package format errors.
     /// @param[in] reader reader of the node package.
     /// @param[in] asset_reader reader of assets, required to read data schema.
     /// @param[in] resource_pkg path to resource package containing data schema.
     /// @param[in] schema_name name of data schema asset in the package.
     /// @param[in] out_file path to output file.
     virtual void convert_node_package( INodePackageReader& reader, IAssetReader& asset_reader,
          const std::filesystem::path& resource_pkg, const std::string& schema_name,
          const std::filesystem::path& out_file ) = 0;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_CONVERTER_H
