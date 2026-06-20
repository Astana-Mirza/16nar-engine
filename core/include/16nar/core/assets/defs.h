/// @file
/// @brief File with basic definitions for the assets component.
#ifndef _16NAR_CORE_ASSETS_DEFS_H
#define _16NAR_CORE_ASSETS_DEFS_H

#include <16nar/core/defs.h>
#include <16nar/platform/memory/defs.h>
#include <16nar/platform/strings/static_name.h>

#include <memory>
#include <cstdint>

#if defined( NARENGINE_WIN_EXPORT )
#    if defined( NARENGINE_ASSETS_FB_EXPORT )
#         define NARENGINE_ASSETS_FB_API __declspec( dllexport )
#    else
#         define NARENGINE_ASSETS_FB_API __declspec( dllimport )
#    endif // defined( NARENGINE_ASSETS_FB_EXPORT )
#    if defined( NARENGINE_ASSETS_JSON_EXPORT )
#         define NARENGINE_ASSETS_JSON_API __declspec( dllexport )
#    else
#         define NARENGINE_ASSETS_JSON_API __declspec( dllimport )
#    endif // defined( NARENGINE_ASSETS_JSON_EXPORT )
#else
#    define NARENGINE_ASSETS_FB_API
#    define NARENGINE_ASSETS_JSON_API
#endif // defined( NARENGINE_WIN_EXPORT )


namespace _16nar::assets
{

/// @brief Identifier of action of asset parsing.
using AssetActionId = std::uint32_t;


/// @brief Data of the asset.
struct AssetData
{
     strings::StaticName type_id{};     ///< identifier of asset data type.
     memory::ConstByteView data{};      ///< binary buffer with asset data.
};


class IAssetReader;
class IAssetWriter;
class IAssetFileProcessor;
class IAssetDataConvertor;

using IAssetReaderPtr = std::shared_ptr< IAssetReader >;
using IAssetWriterPtr = std::shared_ptr< IAssetWriter >;
using IAssetFileProcessorPtr = std::shared_ptr< IAssetFileProcessor >;
using IAssetDataConvertorPtr = std::shared_ptr< IAssetDataConvertor >;

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_DEFS_H
