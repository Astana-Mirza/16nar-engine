/// @file
/// @brief Header file with definition of functions for reading scene in JSON format.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_READ_UTILS_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_READ_UTILS_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>

#include <nlohmann/json.hpp>

namespace _16nar::tools::constructor2d
{

/// @brief Read dependencies of the scene in JSON format.
/// @param[in] json object to read dependencies from.
/// @throws std::runtime_error and implementation-defined exceptions if @b json is malformed.
/// @return dependencies of the scene.
ENGINE_API Dependencies read_scene_deps( const nlohmann::json& json );

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_JSON_READ_UTILS_H
