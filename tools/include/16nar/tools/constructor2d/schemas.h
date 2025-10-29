/// @file
/// @brief Header file functions for creation of schemas.
#ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_SCENES_H
#define _16NAR_TOOLS_CONSTRUCTOR_2D_SCENES_H

#include <16nar/16nardefs.h>
#include <16nar/tools/data_schema.h>
#include <16nar/tools/assets/iprops_writer_factory.h>
#include <16nar/tools/constructor2d/constructor2d_defs.h>

namespace _16nar::tools::constructor2d
{

/// @brief Make default schema of node in constructor2d architecture.
/// @details Caller is responsible for converting @b writer to default values reader.
/// @param[in] type type of node.
/// @param[out] factory factory for writer of default values of schema's items.
/// @return default schema of constructor2d architecture.
ENGINE_API DataSchema make_default_schema( NodeType type, IPropsWriterFactory& factory );


/// @brief Make schema of constructor2d render system's data.
/// @details Caller is responsible for converting @b writer to default values reader.
/// @param[in] type type of render system.
/// @param factory factory for writer of default values of schema's items.
/// @return schema of constructor2d render system's data.
ENGINE_API DataSchema make_render_system_schema( RenderSystemType type, IPropsWriterFactory& factory );

} // namespace _16nar::tools::constructor2d

#endif // #ifndef _16NAR_TOOLS_CONSTRUCTOR_2D_SCENES_H
