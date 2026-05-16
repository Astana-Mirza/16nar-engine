/// @file
/// @brief File with basic definitions for the Core layer.
#ifndef _16NAR_CORE_DEFS_H
#define _16NAR_CORE_DEFS_H

#include <16nar/platform/defs.h>
#include <16nar/core/config.h>

#if defined( NARENGINE_WIN_EXPORT )
#    if defined( NARENGINE_CORE_EXPORT )
#         define NARENGINE_CORE_API __declspec( dllexport )
#    else
#         define NARENGINE_CORE_API __declspec( dllimport )
#    endif // defined( NARENGINE_CORE_EXPORT )
#else
#    define NARENGINE_CORE_API
#endif // defined( NARENGINE_WIN_EXPORT )

#endif // #ifndef _16NAR_CORE_DEFS_H
