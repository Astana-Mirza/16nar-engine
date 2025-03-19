/// @file
/// @brief File with definition for constructor 2D architecture.
#ifndef _16NAR_TOOLS_CONSTRUCTOR2D_DEFS_H
#define _16NAR_TOOLS_CONSTRUCTOR2D_DEFS_H

#include <cstdint>

namespace _16nar::tools::constructor2d
{

/// @brief Type of node on the scene.
enum class NodeType : uint8_t
{
     Node2D,
     SpriteNode
};

/// @brief Type of render system of the scene state.
enum class RenderSystemType : uint8_t
{
     QTreeRenderSystem
};

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR2D_DEFS_H
