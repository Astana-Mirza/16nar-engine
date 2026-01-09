/// @file
/// @brief Header file with FlatBuffersNodePackageReader class definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_NODE_PACKAGE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_NODE_PACKAGE_READER_H

#include <16nar/tools/constructor2d/inode_package_reader.h>

#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_piece_reader.h>
#include <unordered_map>

namespace _16nar::data::constructor2d
{

class NodePackage2D;

} // namespace _16nar::data::constructor2d


namespace _16nar::tools::constructor2d
{

/// @brief Class for reading node package in flatbuffers format.
class ENGINE_API FlatBuffersNodePackageReader : public INodePackageReader
{
public:
     /// @brief Constructor.
     /// @throws std::runtime_error if passed buffer is incorrect.
     /// @param[in] data data of scene buffer.
     /// @param[in] size size of scene buffer.
     FlatBuffersNodePackageReader( const std::byte *data, std::size_t size );

     /// @copydoc INodePackageReader::get_dependencies()
     Dependencies get_dependencies() override;

     /// @copydoc INodePackageReader::get_scene_piece_reader(const std::string&)
     IScenePieceReader& get_scene_piece_reader( const std::string& name ) override;

     /// @copydoc INodePackageReader::get_scene_piece_count()
     std::size_t get_scene_piece_count() override;

     /// @copydoc INodePackageReader::get_scene_piece_names()
     std::vector< std::string > get_scene_piece_names() override;

private:
     std::unordered_map< std::string, FlatBuffersScenePieceReader > pieces_;    ///< scene piece readers with names.
     const data::constructor2d::NodePackage2D *package_;                        ///< package flat buffer.
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_FLATBUFFERS_NODE_PACKAGE_READER_H
