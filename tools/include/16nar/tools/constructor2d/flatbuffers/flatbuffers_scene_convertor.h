/// @file
/// @brief Header file with FlatBuffersSceneConvertor class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_CONVERTOR_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_CONVERTOR_H

#include <16nar/tools/constructor2d/iscene_convertor.h>

namespace _16nar::tools::constructor2d
{

/// @brief Class for converting scenes and node packages to flatbuffers format.
class ENGINE_API FlatBuffersSceneConvertor : public ISceneConvertor
{
public:
     /// @copydoc ISceneConvertor::convert_scene(ISceneReader&, const std::filesystem::path&)
     void convert_scene( ISceneReader& scene_reader, const std::filesystem::path& out_file ) override;

     /// @copydoc ISceneConvertor::convert_node_package(INodePackageReader&, const std::filesystem::path&)
     void convert_node_package( INodePackageReader& reader, const std::filesystem::path& out_file ) override;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_CONVERTOR_H
