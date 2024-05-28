/// @file
/// @brief File with RenderApi class definition.
#ifndef _16NAR_OPENGL_RENDER_API_H
#define _16NAR_OPENGL_RENDER_API_H

#include <16nar/16nardefs.h>

#include <16nar/render/iresource_manager.h>
#include <16nar/render/render_defs.h>
#include <16nar/system/exceptions.h>

#include <unordered_map>
#include <memory>

namespace _16nar::opengl
{

/// @brief Class for rendering primitives with OpenGL.
class ENGINE_API RenderApi
{
public:
     /// @brief Constructor.
     RenderApi();

     /// @brief Load a resource with given parameters.
     /// @param[in] type type of the resource.
     /// @param[in] params parameters for loading a resource.
     /// @return identifier of a resource with type.
     Resource load( ResourceType type, const std::any& params );

     /// @brief Unload a resource.
     /// @param[in] resource identifier of a resource with type.
     void unload( const Resource& resource );

private:
     RenderApi( const RenderApi& ) = delete;
     RenderApi& operator=( const RenderApi& ) = delete;

private:
     ResourceManagerMap managers_;      ///< all resource managers.
};

} // namespace _16nar::opengl

#endif // _16NAR_OPENGL_RENDER_API_H
