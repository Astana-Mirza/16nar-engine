/// @file
/// @brief Header file with JsonSceneStateReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_STATE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_STATE_READER_H

#include <16nar/tools/constructor2d/iscene_state_reader.h>

#include <16nar/tools/assets/json/json_props_reader.h>

namespace _16nar::tools::constructor2d
{

/// @brief Class for reading scene state in JSON format.
class ENGINE_API JsonSceneStateReader : public ISceneStateReader
{
public:
     /// @brief Default constructor.
     JsonSceneStateReader() = default;

     /// @brief Constructor.
     /// @param[in] json object of scene state.
     JsonSceneStateReader( const nlohmann::json& json );

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
     /// @brief Iterator of array of nodes.
     using NodeIter = nlohmann::json::const_iterator;

     const nlohmann::json *json_{};          ///< object of scene state.
     JsonPropsReader render_system_reader_;  ///< reader of render system's properties.
     JsonPropsReader node_reader_;           ///< reader of render system's properties.
     NodeIter begin_;                        ///< begin iterator of nodes array.
     NodeIter end_;                          ///< end iterator of nodes array.
     NodeIter curr_node_;                    ///< current node.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_STATE_READER_H
