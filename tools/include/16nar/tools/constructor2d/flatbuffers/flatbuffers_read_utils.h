/// @file
/// @brief Header file with definition of functions for reading scene in flatbuffers format.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_READ_UTILS_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_READ_UTILS_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>
#include <16nar/tools/constructor2d/constructor2d_defs.h>
#include <16nar/tools/assets/flatbuffers/flatbuffers_props_reader.h>

namespace _16nar::data
{
namespace constructor2d
{

class Dependencies;
class NodeData;
class RenderSystemData;

} // namespace constructor2d

class Properties;

} // namespace _16nar::data


namespace _16nar::tools::constructor2d
{

/// @brief Read dependencies of the scene in flatbuffers format.
/// @throws std::runtime_error if serialized data is incorrect.
/// @param[in] deps object to read dependencies from.
/// @return dependencies of the scene.
ENGINE_API Dependencies read_scene_deps( const _16nar::data::constructor2d::Dependencies *deps );


/// @brief Create properties reader from serialized data.
/// @throws std::runtime_error if serialized data is incorrect.
/// @param[in] props object to read properties from.
/// @return properties reader.
ENGINE_API FlatBuffersPropsReader create_props_reader( const _16nar::data::Properties *props );


/// @brief Make reader of node's properties and its type.
/// @throws std::runtime_error if serialized data is incorrect.
/// @param[in] node serialized node object.
/// @param[out] type type of scene node.
/// @return properties reader.
ENGINE_API FlatBuffersPropsReader make_node_reader(
    const _16nar::data::constructor2d::NodeData *node, NodeType& type );


/// @brief Make reader of render system's properties and its type.
/// @throws std::runtime_error if serialized data is incorrect.
/// @param[in] node serialized render system object.
/// @param[out] type type of render system.
/// @return properties reader.
ENGINE_API FlatBuffersPropsReader make_render_system_reader(
     const _16nar::data::constructor2d::RenderSystemData *render_system, RenderSystemType& type );

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_READ_UTILS_H
