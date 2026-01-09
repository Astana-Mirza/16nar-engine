#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_convertor.h>

#include <16nar/gen/flatbuffers/constructor2d_scene_generated.h>
#include <16nar/gen/flatbuffers/constructor2d_node_pkg_generated.h>

namespace _16nar::tools::constructor2d
{

void FlatBuffersSceneConvertor::convert_scene(
     ISceneReader& scene_reader, const std::filesystem::path& out_file )
{
     /// @todo implement
}


void FlatBuffersSceneConvertor::convert_node_package(
     INodePackageReader& reader, const std::filesystem::path& out_file )
{
     /// @todo implement
}

} // namespace _16nar::tools::constructor2d
