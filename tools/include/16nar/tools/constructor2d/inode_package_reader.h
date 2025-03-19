/// @file
/// @brief Header file with INodePackageReader interface definition.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_INODE_PACKAGE_READER_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_INODE_PACKAGE_READER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>

namespace _16nar::tools::constructor2d
{

/// @brief Interface for reading node package from serialized image.
class ENGINE_API INodePackageReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~INodePackageReader() = default;

     /// @brief Get list of all dependencies of the node package.
     /// @return list of all dependencies of the node package.
     virtual Dependencies get_dependencies() = 0;

     /// @brief Get current scene piece reader.
     /// @details Initially reads first scene piece. To read next one, @b next_piece should be called.
     /// @return current scene piece reader.
     virtual ISceneStateReader& get_current_piece_reader() = 0;

     /// @brief Switch to next scene piece.
     /// @details Call of this function affects result of @b get_current_piece_reader.
     /// If no next scene piece exists, the function returns false and does not make any changes.
     /// @return true if successfully switched to next scene piece, false otherwise.
     virtual bool next_piece() = 0;
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_INODE_PACKAGE_READER_H
