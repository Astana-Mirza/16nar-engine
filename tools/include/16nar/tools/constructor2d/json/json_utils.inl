#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_UTILS_INL
#define _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_UTILS_INL

#include <nlohmann/json.hpp>
#include <16nar/tools/constructor2d/constructor2d_defs.h>

namespace _16nar::tools::constructor2d
{

NLOHMANN_JSON_SERIALIZE_ENUM( NodeType, {
     { NodeType::Node2D,           "node_2d" },
     { NodeType::SpriteNode,       "sprite_node" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( RenderSystemType, {
     { RenderSystemType::QTreeRenderSystem,  "qtree" },
} )

} // namespace _16nar::tools::constructor2d


#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_UTILS_INL
