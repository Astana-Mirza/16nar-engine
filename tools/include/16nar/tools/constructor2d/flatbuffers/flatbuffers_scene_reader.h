/// @file
/// @brief Header file with FlatBuffersSceneReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_READER_H

#include <16nar/tools/constructor2d/iscene_reader.h>

#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_piece_reader.h>
#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_state_reader.h>

#include <vector>
#include <string_view>

namespace _16nar::data::constructor2d
{

class Scene2D;

} // namespace _16nar::data::constructor2d


namespace _16nar::tools::constructor2d
{

class ISceneStateReader;
class IScenePieceReader;

/// @brief Class for reading scene in flatbuffers format.
class ENGINE_API FlatBuffersSceneReader : public ISceneReader
{
public:
     /// @brief Constructor.
     /// @throws std::runtime_error if passed buffer is incorrect.
     /// @param[in] data data of scene buffer.
     /// @param[in] size size of scene buffer.
     FlatBuffersSceneReader( const std::byte *data, std::size_t size );

     /// @copydoc ISceneReader::get_dependencies()
     Dependencies get_dependencies() override;

     /// @copydoc ISceneReader::get_setup_func()
     std::optional< ResourceIndex > get_setup_func() override;

     /// @copydoc ISceneReader::get_loop_func()
     std::optional< ResourceIndex > get_loop_func() override;

     /// @copydoc ISceneReder::is_empty()
     bool is_empty() override;

     /// @copydoc ISceneReader::get_current_state_reader()
     ISceneStateReader& get_current_state_reader() override;

     /// @copydoc ISceneReader::next_state()
     bool next_state() override;

     /// @copydoc ISceneReader::get_scene_piece_reader(std::size_t)
     IScenePieceReader& get_scene_piece_reader( std::size_t idx ) override;

     /// @copydoc ISceneReader::get_scene_piece_count()
     std::size_t get_scene_piece_count() override;

private:
     std::vector< FlatBuffersScenePieceReader > pieces_;    ///< scene piece readers.
     FlatBuffersSceneStateReader state_reader_;             ///< current scene state reader.
     const data::constructor2d::Scene2D *scene_;            ///< scene flat buffer.
     const void *states_;                                   ///< flatbuffers vector of scene states.
     std::size_t begin_index_;                              ///< index of the first state.
     std::size_t end_index_;                                ///< index straight after the index the last state.
     std::size_t curr_index_;                               ///< index of current scene state.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_SCENE_READER_H
