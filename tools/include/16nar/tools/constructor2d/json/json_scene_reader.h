/// @file
/// @brief Header file with JsonSceneReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_READER_H

#include <16nar/tools/constructor2d/iscene_reader.h>

#include <16nar/tools/constructor2d/json/json_scene_piece_reader.h>
#include <16nar/tools/constructor2d/json/json_scene_state_reader.h>

#include <vector>
#include <string_view>

namespace _16nar::tools::constructor2d
{

class ISceneStateReader;
class IScenePieceReader;

/// @brief Class for reading scene in JSON format.
class ENGINE_API JsonSceneReader : public ISceneReader
{
public:
     /// @brief Constructor.
     /// @throws std::runtime_error and implementation-defined exceptions if @b json is malformed.
     /// @param[in] json object with node package.
     JsonSceneReader( const nlohmann::json& json );

     /// @brief Constructor with object move.
     /// @param[in] json object with node package.
     JsonSceneReader( nlohmann::json&& json );

     /// @copydoc ISceneReader::get_dependencies()
     Dependencies get_dependencies() override;

     /// @copydoc ISceneReader::get_setup_func()
     ResourceIndex get_setup_func() override;

     /// @copydoc ISceneReader::get_loop_func()
     ResourceIndex get_loop_func() override;

     /// @copydoc ISceneReader::get_schema()
     ResourceIndex get_schema() override;

     /// @copydoc ISceneReader::get_current_state_reader()
     ISceneStateReader& get_current_state_reader() override;

     /// @copydoc ISceneReader::next_state()
     bool next_state() override;

     /// @copydoc ISceneReader::get_scene_piece_reader(std::size_t)
     IScenePieceReader& get_scene_piece_reader( std::size_t idx ) override;

     /// @copydoc ISceneReader::get_scene_piece_count()
     std::size_t get_scene_piece_count() override;

private:
     /// @brief Read JSON object to fill data fields.
     /// @throws std::runtime_error and implementation-defined exceptions if @b json_ is malformed.
     void read_json();

     /// @brief Read resource index with given @b name.
     /// @param[in] name name of JSON field.
     /// @param[in] optional true if index may be absent, false otherwise.
     /// @return resource index read, will be zeroed if absent in scene and @b optional is true.
     ResourceIndex read_resource_index( std::string_view name, bool optional ) const;

private:
     /// @brief Iterator of array of nodes.
     using NodeIter = nlohmann::json::const_iterator;

     nlohmann::json json_;                        ///< object of the scene.
     std::vector< JsonScenePieceReader > pieces_; ///< scene piece readers.
     JsonSceneStateReader state_reader_;          ///< current scene state reader.
     NodeIter end_;                               ///< end iterator of scene states array.
     NodeIter curr_state_;                        ///< current scene state.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_SCENE_READER_H
