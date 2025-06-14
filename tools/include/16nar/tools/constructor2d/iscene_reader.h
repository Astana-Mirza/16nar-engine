/// @file
/// @brief Header file with ISceneReader interface definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_READER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>

namespace _16nar::tools::constructor2d
{

class ISceneStateReader;
class IScenePieceReader;

/// @brief Interface for reading scene from serialized image.
class ENGINE_API ISceneReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~ISceneReader() = default;

     /// @brief Get list of all dependencies of the scene.
     /// @return list of all dependencies of the scene.
     virtual Dependencies get_dependencies() = 0;

     /// @brief Get resource index of setup function.
     /// @return resource index of setup function, will be zeroed if absent in scene.
     virtual ResourceIndex get_setup_func() = 0;

     /// @brief Get resource index of loop function.
     /// @return resource index of loop function, will be zeroed if absent in scene.
     virtual ResourceIndex get_loop_func() = 0;

     /// @brief Get resource index of scene nodes' data schema.
     /// @return resource index of scene nodes' data schema.
     virtual ResourceIndex get_schema() = 0;

     /// @brief Get current scene state reader.
     /// @details Initially reads first scene state. To read next one, @b next_state should be called.
     /// @return current scene state reader.
     virtual ISceneStateReader& get_current_state_reader() = 0;

     /// @brief Switch to next scene state.
     /// @details Call of this function affects result of @b get_current_state_reader.
     /// If no next state exists, the function returns false and does not make any changes.
     /// @return true if successfully switched to next state, false otherwise.
     virtual bool next_state() = 0;

     /// @brief Get scene piece reader at given index.
     /// @param[in] idx index of the scene piece reader.
     /// @return scene piece reader.
     virtual IScenePieceReader& get_scene_piece_reader( std::size_t idx ) = 0;

     /// @brief Get number of scene pieces in this package.
     /// @return number of scene pieces in this package.
     virtual std::size_t get_scene_piece_count() = 0;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_ISCENE_READER_H
