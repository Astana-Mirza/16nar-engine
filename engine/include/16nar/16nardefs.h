/// @file
/// @brief File with basic definitions, needed by engine.
#ifndef _16NAR_DEFS_H
#define _16NAR_DEFS_H

#include <16nar/16narconfig.h>
#include <cstdint>

#if defined( NARENGINE_WIN_EXPORT )
#    ifdef NARENGINE_DLL
#         define ENGINE_API __declspec( dllexport )
#    else
#         define ENGINE_API __declspec( dllimport )
#    endif
#else
#    define ENGINE_API
#endif

namespace _16nar
{

/// @brief Type of resource identifier.
using ResID = uint32_t;


/// @brief Type of application profile, representing its execution method.
enum class ProfileType
{
     SingleThreaded,     ///< executing application in single thread. 
     MultiThreaded       ///< executing appliction in multiple threads, each having its own purpose.
};


/// @brief Type of resource used for render.
enum class ResourceType
{
     Texture,            ///< 2D texture.
     Buffer,             ///< buffer (for rendering).
     Shader,             ///< shader program.
     FrameBuffer,        ///< buffer for the frame.
     VertexBuffer,       ///< buffer or group of buffers for vertices.
     Unknown             ///< unknown resource type for default initialization.
};


/// @brief Resource used in engine.
struct Resource
{
     ResourceType type = ResourceType::Unknown; ///< type of the resource.
     ResID        id   = 0;                     ///< id of the resource.

     bool operator==( const Resource& rhs ) const { return type == rhs.type && id == rhs.id; }
     bool operator!=( const Resource& rhs ) const { return !( *this == rhs ); }
};


/// @brief Resource with type embedded in typename.
/// @tparam R type of resource.
template < ResourceType R >
struct TypedResource : public Resource
{
     TypedResource()                                      { type = R; }
     TypedResource( const TypedResource& other )          { type = R; id = other.id; }
     explicit TypedResource( ResID resId )                { type = R; id = resId; }
     TypedResource& operator=( const TypedResource& rhs ) { type = R; id = rhs.id; return *this; }
     TypedResource& operator=( ResID resId )              { type = R; id = resId; return *this; }
     bool operator==( const TypedResource& rhs ) const    { return id == rhs.id; }
     bool operator!=( const TypedResource& rhs ) const    { return !( *this == rhs ); }
};


using Shader       = TypedResource< ResourceType::Shader       >;
using Texture      = TypedResource< ResourceType::Texture      >;
using Buffer       = TypedResource< ResourceType::Buffer       >;
using FrameBuffer  = TypedResource< ResourceType::FrameBuffer  >;
using VertexBuffer = TypedResource< ResourceType::VertexBuffer >;

} // nmespace _16nar

#endif // #ifndef _16NAR_DEFS_H
