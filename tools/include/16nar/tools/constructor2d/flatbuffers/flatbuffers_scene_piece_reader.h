/// @file
/// @brief Header file with FlatBuffersScenePieceReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_PIECE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_PIECE_READER_H

#include <16nar/tools/constructor2d/iscene_piece_reader.h>
#include <16nar/tools/assets/flatbuffers/flatbuffers_props_reader.h>

namespace _16nar::data::constructor2d
{

class ScenePiece2D;

} // namespace _16nar::data::constructor2d


namespace _16nar::tools::constructor2d
{

/// @brief Class for reading scene piece in flatbuffers format.
class ENGINE_API FlatBuffersScenePieceReader : public IScenePieceReader
{
public:
     /// @brief Default constructor.
     FlatBuffersScenePieceReader();

     /// @brief Constructor.
     /// @throws std::runtime_error if serialized data is incorrect.
     /// @param[in] piece serialized scene piece.
     FlatBuffersScenePieceReader( const data::constructor2d::ScenePiece2D *piece );

     /// @copydoc IScenePieceReader::is_empty()
     bool is_empty() override;

     /// @copydoc IScenePieceReader::get_current_node_reader()
     IPropsReader& get_current_node_reader() override;

     /// @copydoc IScenePieceReader::get_current_node_type()
     NodeType get_current_node_type() override;

     /// @copydoc IScenePieceReader::next_node
     bool next_node() override;

private:
     /// @brief Initialization of current node's properties reader.
     /// @throws std::runtime_error if serialized data is incorrect.
     void init_current_node_reader();

private:
     FlatBuffersPropsReader node_reader_{};            ///< reader of current node.
     const void *nodes_{};                             ///< flatbuffers vector of scene nodes.
     std::size_t begin_index_{};                       ///< index of the first node.
     std::size_t end_index_{};                         ///< index straight after the index the last node.
     std::size_t curr_index_{};                        ///< index of current scene node.
     NodeType curr_node_type_{ NodeType::Node2D };     ///< type of current node.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_PIECE_READER_H
