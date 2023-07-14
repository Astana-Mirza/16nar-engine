/// @file
/// @brief File with basic definitions, needed by engine.
#ifndef _16NAR_DEFS_H
#define _16NAR_DEFS_H

#include <16nar/16narconfig.h>

#ifdef NARENGINE_RENDER_OPENGL
#    define GLM_FORCE_INLINE
#    define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#    define GLM_FORCE_INTRINSICS
#    define GLM_FORCE_SIZE_T_LENGTH
#endif


#ifdef NARENGINE_WIN_EXPORT
#    ifdef BUILDING_DLL
#         define ENGINE_API __declspec( dllexport )
#    else
#         define ENGINE_API __declspec( dllimport )
#    endif
#else
#    define ENGINE_API
#endif


namespace _16nar
{

class View;

} // namespace _16nar

#endif // #ifndef _16NAR_DEFS_H
