/// @file
/// @brief Header file with ISceneStateReader interface definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_STATE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_STATE_READER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>
#include <16nar/tools/constructor2d/constructor2d_defs.h>

namespace _16nar::tools::constructor2d
{

class IPropsReader;

/// @brief Interface for reading scene state from serialized image.
class ENGINE_API ISceneStateReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~ISceneStateReader() = default;

     /// @brief Get rendering flag of the scene state.
     /// @return rendering flag of the scene state.
     virtual bool get_rendering() = 0;

     /// @brief Get updating flag of the scene state.
     /// @return updating flag of the scene state.
     virtual bool get_updating() = 0;

     /// @brief Get render system's properties reader.
     /// @return render system's properties reader.
     virtual IPropsReader& get_render_system_reader() = 0;

     /// @brief Get type of render system.
     /// @return type of render system.
     virtual RenderSystemType get_render_system_type() = 0;

     /// @brief Get current node's properties reader.
     /// @details Initially reads first node. To read next one, @b next_node should be called.
     /// @return current node's properties reader.
     virtual IPropsReader& get_current_node_reader() = 0;

     /// @brief Get current node type.
     /// @return current node type.
     virtual NodeType get_current_node_type() = 0;

     /// @brief Switch to next node.
     /// @details Call of this function affects result of @b get_current_node_reader and @b get_current_node_type.
     /// If no next node exists, the function returns false and does not make any changes.
     /// @return true if successfully switched to next node, false otherwise.
     virtual bool next_node() = 0;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_STATE_READER_H
