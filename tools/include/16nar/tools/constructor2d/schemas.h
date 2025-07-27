/// @file
/// @brief Header file functions for creation of schemas.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_SCENES_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_SCENES_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>
#include <16nar/tools/assets/iprops_writer.h>
#include <16nar/tools/constructor2d/constructor2d_defs.h>

namespace _16nar::tools::constructor2d
{

/// @brief Make default schema of constructor2d architecture.
/// @details Caller is responsible for converting @b writer to default values reader.
/// @param[out] writer writer of default values of schema's items.
/// @return default schema of constructor2d architecture.
ENGINE_API DataSchema make_default_schema( IPropsWriter& writer );


/// @brief Make schema of constructor2d render system's data.
/// @details Caller is responsible for converting @b writer to default values reader.
/// @param[in] type type of render system.
/// @param writer writer of default values of schema's items.
/// @return schema of constructor2d render system's data.
ENGINE_API DataSchema make_render_system_schema( RenderSystemType type, IPropsWriter& writer );

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_SCENES_H
