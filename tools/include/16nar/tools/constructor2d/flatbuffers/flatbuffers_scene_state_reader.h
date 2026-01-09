/// @file
/// @brief Header file with FlatBuffersSceneStateReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_STATE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_STATE_READER_H

#include <16nar/tools/constructor2d/iscene_state_reader.h>

#include <16nar/tools/assets/flatbuffers/flatbuffers_props_reader.h>

namespace _16nar::data::constructor2d
{

class SceneState2D;

} // namespace _16nar::data::constructor2d


namespace _16nar::tools::constructor2d
{

/// @brief Class for reading scene state in flatbuffers format.
class ENGINE_API FlatBuffersSceneStateReader : public ISceneStateReader
{
public:
     /// @brief Default constructor.
     FlatBuffersSceneStateReader();

     /// @brief Constructor.
     /// @throws std::runtime_error if serialized data is incorrect.
     /// @param[in] state serialized scene state.
     FlatBuffersSceneStateReader( const data::constructor2d::SceneState2D *state );

     /// @copydoc ISceneStateReader::get_rendering()
     bool get_rendering() override;

     /// @copydoc ISceneStateReader::get_updating()
     bool get_updating() override;

     /// @copydoc ISceneStateReader::get_render_system_reader()
     IPropsReader& get_render_system_reader() override;

     /// @copydoc ISceneStateReader::get_render_system_type()
     RenderSystemType get_render_system_type() override;

     /// @copydoc ISceneStateReader::is_empty()
     bool is_empty() override;

     /// @copydoc ISceneStateReader::get_current_node_reader()
     IPropsReader& get_current_node_reader() override;

     /// @copydoc ISceneStateReader::get_current_node_type()
     NodeType get_current_node_type() override;

     /// @copydoc ISceneStateReader::next_node()
     bool next_node() override;

private:
     /// @brief Initialization of current node's properties reader.
     /// @throws std::runtime_error if serialized data is incorrect.
     void init_current_node_reader();

private:
     FlatBuffersPropsReader render_system_reader_{};   ///< reader of render system's properties.
     FlatBuffersPropsReader node_reader_{};            ///< reader of nodes's properties.
     const void *nodes_{};                             ///< flatbuffers vector of scene nodes.
     std::size_t begin_index_{};                       ///< index of the first node.
     std::size_t end_index_{};                         ///< index straight after the index the last node.
     std::size_t curr_index_{};                        ///< index of current scene node.
     NodeType curr_node_type_{ NodeType::Node2D };     ///< type of current node.
     RenderSystemType render_system_type_{ RenderSystemType::QTreeRenderSystem };    ///< type of render system.
     bool rendering_{};                                ///< rendering flag of the scene state.
     bool updating_{};                                 ///< updating flag of the scene state.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_STATE_READER_H
