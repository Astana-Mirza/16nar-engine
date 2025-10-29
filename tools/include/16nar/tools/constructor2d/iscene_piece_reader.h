/// @file
/// @brief Header file with IScenePieceReader interface definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_PIECE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_PIECE_READER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/assets/iprops_reader.h>
#include <16nar/tools/constructor2d/constructor2d_defs.h>

namespace _16nar::tools::constructor2d
{

/// @brief Interface for reading scene piece from serialized image.
class ENGINE_API IScenePieceReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IScenePieceReader() = default;

     /// @brief Check if there are no nodes to read.
     /// @return true if there are no nodes to read, false otherwise. 
     virtual bool is_empty() = 0;

     /// @brief Get current node's properties reader.
     /// @details Initially reads first node. To read next one, @b next_node should be called.
     /// @return current node's properties reader.
     virtual IPropsReader& get_current_node_reader() = 0;

     /// @brief Get current node type.
     /// @return current node type.
     virtual NodeType get_current_node_type() = 0;

     /// @brief Switch to next scene state.
     /// @details Call of this function affects result of @b get_current_node_reader and @b get_current_node_type.
     /// If no next state exists, the function returns false and does not make any changes.
     /// @return true if successfully switched to next state, false otherwise.
     virtual bool next_node() = 0;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_PIECE_READER_H
