/// @file
/// @brief Header file with VertexBufferLoader class definition.
#ifndef _16NAR_OPENGL_VERTEX_BUFFER_LOADER_H
#define _16NAR_OPENGL_VERTEX_BUFFER_LOADER_H

#include <16nar/render/opengl/utils.h>

namespace _16nar::opengl
{

/// @brief Class for loading the vertex buffer.
class VertexBufferLoader
{
public:
     /// @brief Parameters of vertex buffer loading.
     using LoadParamsType = LoadParams< ResourceType::VertexBuffer >;

     /// @brief Handler of vertex buffer.
     using HandlerType = Handler< ResourceType::VertexBuffer >;

     /// @brief Load vertex buffer to OpenGL and fill its handler.
     /// @param[in] params parameters of vertex buffer loading.
     /// @param[out] handler handler of the vertex buffer.
     /// @return true on success, false otherwise.
     static bool load( const ResourceManagerMap&, const LoadParamsType& params, HandlerType& handler );

     /// @brief Unload vertex buffer from OpenGL.
     /// @param[in] handler handler of the vertex buffer.
     /// @return true on success, false otherwise.
     static bool unload( const HandlerType& handler );
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_VERTEX_BUFFER_LOADER_H
