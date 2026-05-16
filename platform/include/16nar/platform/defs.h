/// @file
/// @brief File with basic definitions for the Platform layer.
#ifndef _16NAR_PLATFORM_DEFS_H
#define _16NAR_PLATFORM_DEFS_H

#include <16nar/platform/config.h>

#include <cstdint>

#define NARENGINE_VERSION_TO_UINT32( X, Y, Z ) \
     ( ( static_cast< std::uint32_t >( X ) << std::uint32_t{ 16 } ) | \
     ( static_cast< std::uint32_t >( Y ) << std::uint32_t{ 8 } ) | \
     static_cast< std::uint32_t >( Z ) )

#define NARENGINE_VERSION_UINT32 \
     NARENGINE_VERSION_TO_UINT32( NARENGINE_VERSION_MAJOR, \
     NARENGINE_VERSION_MINOR, NARENGINE_VERSION_PATCH )

#define NARENGINE_ASSET_VERSION_UINT32 \
     NARENGINE_VERSION_TO_UINT32( NARENGINE_ASSET_VERSION_MAJOR, \
     NARENGINE_ASSET_VERSION_MINOR, NARENGINE_ASSET_VERSION_PATCH )

#define NARENGINE_VERSION_COMPATIBLE_MASK ( ~std::uint32_t{ 255 } )
#define NARENGINE_VERSION_CHECK( X, Y ) ( !( ( ( X ) ^ ( Y ) ) & NARENGINE_VERSION_COMPATIBLE_MASK ) )


#if defined( NARENGINE_WIN_EXPORT )
#    if defined( NARENGINE_PLATFORM_EXPORT )
#         define NARENGINE_PLATFORM_API __declspec( dllexport )
#    else
#         define NARENGINE_PLATFORM_API __declspec( dllimport )
#    endif // defined( NARENGINE_PLATFORM_EXPORT )
#else
#    define NARENGINE_PLATFORM_API
#endif // defined( NARENGINE_WIN_EXPORT )

#endif // #ifndef _16NAR_PLATFORM_DEFS_H
