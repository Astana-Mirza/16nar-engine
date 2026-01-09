/// @file
/// @brief Header file with JsonNodePackageReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_NODE_PACKAGE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_NODE_PACKAGE_READER_H

#include <16nar/tools/constructor2d/inode_package_reader.h>

#include <16nar/tools/constructor2d/json/json_scene_piece_reader.h>
#include <unordered_map>

namespace _16nar::tools::constructor2d
{

/// @brief Class for reading node package in JSON format.
class ENGINE_API JsonNodePackageReader : public INodePackageReader
{
public:
     /// @brief Constructor.
     /// @throws std::runtime_error and implementation-defined exceptions if @b json is malformed.
     /// @param[in] json object with node package.
     explicit JsonNodePackageReader( const nlohmann::json& json );

     /// @brief Constructor with object move.
     /// @param[in] json object with node package.
     explicit JsonNodePackageReader( nlohmann::json&& json );

     /// @copydoc INodePackageReader::get_dependencies()
     Dependencies get_dependencies() override;

     /// @copydoc INodePackageReader::get_scene_piece_reader(const std::string&)
     IScenePieceReader& get_scene_piece_reader( const std::string& name ) override;

     /// @copydoc INodePackageReader::get_scene_piece_count()
     std::size_t get_scene_piece_count() override;

     /// @copydoc INodePackageReader::get_scene_piece_names()
     std::vector< std::string > get_scene_piece_names() override;

private:
     /// @brief Read JSON object to fill data fields.
     /// @throws std::runtime_error and implementation-defined exceptions if @b json_ is malformed.
     void read_json();

private:
     nlohmann::json json_;                                            ///< object with node package.
     std::unordered_map< std::string, JsonScenePieceReader > pieces_; ///< scene piece readers with names.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_NODE_PACKAGE_READER_H
