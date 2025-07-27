/// @file
/// @brief Header file with JsonSceneConvertor class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_CONVERTOR_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_CONVERTOR_H

#include <16nar/tools/constructor2d/iscene_converter.h>

namespace _16nar::tools::constructor2d
{

/// @brief Class for converting scenes and node packages to JSON format.
class ENGINE_API JsonSceneConverter : public ISceneConverter
{
public:
     /// @copydoc ISceneConvertor::convert_scene(ISceneReader&, IAssetReader&,
     ///    const std::filesystem::path&, const std::filesystem::path&)
     void convert_scene( ISceneReader& scene_reader, IAssetReader& asset_reader,
          const std::filesystem::path& resource_dir, const std::filesystem::path& out_file ) override;

     /// @copydoc ISceneConvertor::convert_node_package(INodePackageReader&, IAssetReader&,
     ///    const std::filesystem::path&, const std::string&, const std::filesystem::path&)
     void convert_node_package( INodePackageReader& reader, IAssetReader& asset_reader,
          const std::filesystem::path& resource_pkg, const std::string& schema_name,
          const std::filesystem::path& out_file ) override;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_CONVERTOR_H
