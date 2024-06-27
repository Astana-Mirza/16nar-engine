/// @file
/// @brief Header file with RenderBufferLoader class definition.
#ifndef _16NAR_OPENGL_RENDER_BUFFER_LOADER_H
#define _16NAR_OPENGL_RENDER_BUFFER_LOADER_H

#include <16nar/render/opengl/utils.h>

#include <vector>

namespace _16nar::opengl
{

/// @brief Class for loading the render buffer.
class RenderBufferLoader
{
public:
     /// @brief Parameters of render buffer loading.
     using LoadParams = LoadParams< ResourceType::RenderBuffer >;

     /// @brief Handler of loaded render buffer and its attachments.
     using Handler = Handler< ResourceType::RenderBuffer >;

     /// @brief Load render buffer to OpenGL and fill its handler.
     /// @param[in] params parameters of render buffer loading.
     /// @param[out] handler handler of the render buffer.
     /// @return true on success, false otherwise.
     static bool load( const ResourceManagerMap&, const LoadParams& params, Handler& handler );

     /// @brief Unload render buffer from OpenGL.
     /// @param[in] handler handler of the render buffer.
     /// @return true on success, false otherwise.
     static bool unload( const Handler& handler );

};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_RENDER_BUFFER_LOADER_H