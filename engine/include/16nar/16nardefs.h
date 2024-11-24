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
     MultiThreaded,      ///< executing appliction in multiple threads, each having its own purpose.
     Unknown             ///< unknown profile, set on error.
};


/// @brief Type of render API.
enum class RenderApiType
{
#if defined( NARENGINE_RENDER_OPENGL )
     OpenGl,             ///< OpenGL render API.
#endif // NARENGINE_RENDER_OPENGL
     Unknown             ///< unknown render API, set on error.
};


/// @brief Type of resource used for render.
enum class ResourceType
{
     Texture,            ///< 2D texture.
     Shader,             ///< shader program.
     FrameBuffer,        ///< buffer for the frame.
     VertexBuffer,       ///< buffer or group of buffers for vertices.
     RenderBuffer,       ///< write-only render buffer.
     Cubemap,            ///< 3D texture.
     Unknown             ///< unknown resource type for default initialization.
};


/// @brief Resource used in engine.
struct Resource
{
     ResourceType type = ResourceType::Unknown; ///< type of the resource.
     ResID        id   = 0;                     ///< id of the resource.

     inline bool operator==( const Resource& rhs ) const noexcept { return type == rhs.type && id == rhs.id; }
     inline bool operator!=( const Resource& rhs ) const noexcept { return !( *this == rhs ); }
};


/// @brief Resource with type embedded in typename.
/// @tparam R type of resource.
template < ResourceType R >
struct TypedResource : public Resource
{
     inline TypedResource() noexcept                                       { type = R; }
     inline TypedResource( const TypedResource& other ) noexcept           { type = R; id = other.id; }
     inline explicit TypedResource( ResID resId ) noexcept                 { type = R; id = resId; }
     inline TypedResource& operator=( const TypedResource& rhs ) noexcept  { type = R; id = rhs.id; return *this; }
     inline TypedResource& operator=( ResID resId ) noexcept               { type = R; id = resId; return *this; }
     inline bool operator==( const TypedResource& rhs ) const noexcept     { return id == rhs.id; }
     inline bool operator!=( const TypedResource& rhs ) const noexcept     { return !( *this == rhs ); }
};


using Shader       = TypedResource< ResourceType::Shader       >;
using Texture      = TypedResource< ResourceType::Texture      >;
using FrameBuffer  = TypedResource< ResourceType::FrameBuffer  >;
using VertexBuffer = TypedResource< ResourceType::VertexBuffer >;

} // namespace _16nar


namespace std
{

template< typename Key > struct hash;

/// @brief Definition of hash for pair of std::type_index and pointer, needed for signals.
template<>
struct hash< ::_16nar::Resource >
{
     inline size_t operator()( const ::_16nar::Resource& id ) const noexcept
     {
          return id.id ^ static_cast< ::_16nar::ResID >( id.type );
     }
};

}

#endif // #ifndef _16NAR_DEFS_H
