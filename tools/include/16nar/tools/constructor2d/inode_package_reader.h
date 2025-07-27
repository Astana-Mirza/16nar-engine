/// @file
/// @brief Header file with INodePackageReader interface definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_INODE_PACKAGE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_INODE_PACKAGE_READER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>

namespace _16nar::tools::constructor2d
{

class IScenePieceReader;

/// @brief Interface for reading node package from serialized image.
class ENGINE_API INodePackageReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~INodePackageReader() = default;

     /// @brief Get list of all dependencies of the node package.
     /// @return list of all dependencies of the node package.
     virtual Dependencies get_dependencies() = 0;

     /// @brief Get scene piece reader of scene piece with given name.
     /// @param[in] name name of the scene piece.
     /// @return scene piece reader.
     virtual IScenePieceReader& get_scene_piece_reader( const std::string& name ) = 0;

     /// @brief Get number of scene pieces in this package.
     /// @return number of scene pieces in this package.
     virtual std::size_t get_scene_piece_count() = 0;

     /// @brief Get names of scene pieces in this package.
     /// @return names of scene pieces in this package.
     virtual std::vector< std::string > get_scene_piece_names() = 0;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_INODE_PACKAGE_READER_H
