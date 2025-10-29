/// @file
/// @brief Header file with ISceneConvertor interface definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_CONVERTOR_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_CONVERTOR_H

#include <16nar/16nardefs.h>
#include <16nar/tools/assets/iasset_reader.h>
#include <16nar/tools/constructor2d/iscene_reader.h>
#include <16nar/tools/constructor2d/inode_package_reader.h>

#include <filesystem>

namespace _16nar::tools::constructor2d
{

/// @brief Interface for converting scenes and node packages to specified format.
class ENGINE_API ISceneConvertor
{
public:
     /// @brief Convert the scene and write result to file.
     /// @throws std::runtime_error in case of scene format errors.
     /// @param[in] scene_reader reader of the scene.
     /// @param[in] out_file path to output file.
     virtual void convert_scene( ISceneReader& scene_reader, const std::filesystem::path& out_file ) = 0;

     /// @brief Convert the node package and write result to file.
     /// @throws std::runtime_error in case of node package format errors.
     /// @param[in] reader reader of the node package.
     /// @param[in] out_file path to output file.
     virtual void convert_node_package( INodePackageReader& reader, const std::filesystem::path& out_file ) = 0;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_CONVERTOR_H
