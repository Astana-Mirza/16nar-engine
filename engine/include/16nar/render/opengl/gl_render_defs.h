/// @file
/// @brief File with basic definitions, needed for render with OpenGL.
#ifndef _16NAR_GL_RENDER_DEFS_H
#define _16NAR_GL_RENDER_DEFS_H

#include <16nar/render/render_defs.h>

#include <vector>

namespace _16nar::opengl
{

/// @brief Handler of a resource.
/// @tparam T type of resource.
template < ResourceType T > struct Handler {};


/// @brief Handler of texture.
template <>
struct Handler< ResourceType::Texture >
{
     unsigned int descriptor = 0;  ///< texture descriptor.
};


/// @brief Handler of framebuffer.
template <>
struct Handler< ResourceType::FrameBuffer >
{
     std::vector< unsigned int > attachments;     ///< attachments descriptors.
     unsigned int descriptor = 0;                 ///< framebuffer descriptor.
};


/// @brief Handler of shader program.
template <>
struct Handler< ResourceType::Shader >
{
     unsigned int descriptor = 0;  ///< shader descriptor.
};


/// @brief Handler of vertex buffer.
template <>
struct Handler< ResourceType::VertexBuffer >
{
     unsigned int vbo_descriptor = 0;   ///< vertex buffer object descriptor.
     unsigned int vao_descriptor = 0;   ///< vertex array object descriptor.
     unsigned int ebo_descriptor = 0;   ///< element buffer object descriptor.
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_GL_RENDER_DEFS_H
