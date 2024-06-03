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
     /// @brief Handler of framebuffer attachment.
     struct Attachment
     {
          unsigned int descriptor;                ///< descriptor of the attachments.
          StorageType type;                       ///< type of attachment storage.
     };
     std::vector< Attachment > attachments;       ///< framebuffer attachments information.
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


/// @brief Convert TextureWrap value to unsigned integer for OpenGL API.
/// @param[in] wrap texture wrap.
/// @return value acceptable by OpenGL.
unsigned int tex_wrap_to_int( TextureWrap wrap );

/// @brief Convert TextureFilter value to unsigned integer for OpenGL API.
/// @param[in] filter texture filter.
/// @return value acceptable by OpenGL.
unsigned int tex_filter_to_int( TextureFilter filter );

/// @brief Convert BufferDataFormat value to unsigned integer for OpenGL API.
/// @param[in] format data format.
/// @return value acceptable by OpenGL.
unsigned int data_format_to_int( BufferDataFormat format );

/// @brief Convert DataType value to unsigned integer for OpenGL API.
/// @param[in] type data type.
/// @return value acceptable by OpenGL.
unsigned int data_type_to_int( DataType type );

/// @brief Convert BufferType value to unsigned integer for OpenGL API.
/// @param[in] type buffer type.
/// @return value acceptable by OpenGL.
unsigned int buffer_type_to_int( BufferType type );

/// @brief Convert PrimitiveType value to unsigned integer for OpenGL API.
/// @param[in] type render primitive type.
/// @return value acceptable by OpenGL.
unsigned int primitive_type_to_int( PrimitiveType type );

/// @brief Get default internal data format for OpenGL API.
/// @param[in] format base data format.
/// @param[in] data_type type of buffer data.
/// @return value acceptable by OpenGL.
unsigned int default_internal_format( BufferDataFormat format, DataType data_type );

/// @brief Get framebuffer attachment type for OpenGL API.
/// @param[in] type type of framebuffer attachment.
/// @param[in] order number of attachment, for color attachment only.
/// @return value acceptable by OpenGL.
unsigned int attachment_type_to_int( AttachmentType type, std::size_t order );

/// @brief Get shader type for OpenGL API.
/// @param[in] type type of shader.
/// @return value acceptable by OpenGL.
unsigned int shader_type_to_int( ShaderType type );

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_GL_RENDER_DEFS_H
