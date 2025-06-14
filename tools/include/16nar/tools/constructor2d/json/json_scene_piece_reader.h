/// @file
/// @brief Header file with JsonScenePieceReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_PIECE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_PIECE_READER_H

#include <16nar/tools/constructor2d/iscene_piece_reader.h>
#include <16nar/tools/assets/json/json_props_reader.h>

#include <vector>

namespace _16nar::tools::constructor2d
{

/// @brief Class for reading scene piece in JSON format.
class ENGINE_API JsonScenePieceReader : public IScenePieceReader
{
public:
     /// @brief Constructor.
     /// @param[in] nodes array of nodes of the scene piece.
     JsonScenePieceReader( const nlohmann::json& nodes = {} );

     /// @copydoc IScenePieceReader::is_empty()
     bool is_empty() override;

     /// @copydoc IScenePieceReader::get_current_node_reader()
     IPropsReader& get_current_node_reader() override;

     /// @copydoc IScenePieceReader::get_current_node_type()
     NodeType get_current_node_type() override;

     /// @copydoc IScenePieceReader::next_node
     bool next_node() override;

private:
     /// @brief Iterator of array of nodes.
     using NodeIter = nlohmann::json::const_iterator;

     JsonPropsReader node_reader_;           ///< reader of current node.
     NodeIter begin_;                        ///< begin iterator of scene piece.
     NodeIter end_;                          ///< end iterator of scene piece.
     NodeIter curr_node_;                    ///< current node.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_PIECE_READER_H
