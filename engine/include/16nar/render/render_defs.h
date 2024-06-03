/// @file
/// @brief File with basic definitions, needed for render.
#ifndef _16NAR_RENDER_DEFS_H
#define _16NAR_RENDER_DEFS_H

#include <16nar/16nardefs.h>
#include <16nar/math/vec.h>
#include <16nar/render/iresource_manager.h>

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>

namespace _16nar
{

class IShader; // forward declaration

/// @brief Map of resource types to respective resource manager.
using ResourceManagerMap = std::unordered_map< ResourceType, std::unique_ptr< IResourceManager > >;

/// @brief Frames saved for profiles with multiple threads.
constexpr std::size_t _16nar_saved_frames = 2;

/// @brief Method of texture wrapping.
/// @details Wrapping is used when rendering resource with
/// resource coordinates outside the range [0;1].
enum class TextureWrap
{
     Repeat,                  ///< repeat the resource image.
     MirroredRepeat,          ///< repeat the image, but mirror with every repeat.
     ClampToEdge,             ///< clamp coordinates into range [0;1].
     ClampToBorder            ///< give specific border color for coordinates outside range [0;1].
};


/// @brief Method of texture filtering.
/// @details Filtering is used when target render size
/// does not exactly match the resource size.
enum class TextureFilter
{
     Nearest,                 ///< the nearest texel color.
     Linear,                  ///< average between closest texel colors.
     NearestMipmapNearest,    ///< nearest texel color on the nearest mipmap.
     NearestMipmapLinear,     ///< average of the nearest texel colors on two nearest mipmaps.
     LinearMipmapNearest,     ///< average between closest texel colors on the nearest mipmap.
     LinearMipmapLinear       ///< average of average of nearest texel colors on two nearest mipmaps.
};


/// @brief Format of buffer data representation.
enum class BufferDataFormat
{
     Rgb,                ///< RGB.
     Rgba,               ///< RGB with alpha channel.
     Srgb,               ///< standard RGB.
     Srgba,              ///< standard RGB with alpha channel.
     Depth,              ///< depth buffer.
     DepthStencil        ///< depth and stencil buffer.
};


/// @brief Type of data representing pixels.
enum class DataType
{
     Byte,               ///< buffer is array of bytes (unsigned chars).
     Float               ///< buffer is array of floats.
};


/// @brief Type of graphics data storage.
enum class StorageType
{
     RenderBuffer,       ///< renderbuffer, cannot be read from.
     Texture2D,          ///< simple 2D texture.
     Texture3D           ///< cubemap.
};


/// @brief Type of attachment to framebuffer.
enum class AttachmentType
{
     Color,
     Depth,
     DepthStencil
};


/// @brief Type of buffer indicating purpose of its data.
enum class BufferType
{
     StreamDraw,    ///< will be modified by app once, drawn by library a few times.
     StreamRead,    ///< will be modified by library once, read by app a few times.
     StreamCopy,    ///< will be modified by library once, drawn by library a few times.

     StaticDraw,    ///< will be modified by app once, drawn by library many times.
     StaticRead,    ///< will be modified by library once, read by app many times.
     StaticCopy,    ///< will be modified by library once, drawn by library many times.

     DynamicDraw,   ///< will be modified by app many times, drawn by library many times.
     DynamicRead,   ///< will be modified by library many times, read by app many times.
     DynamicCopy    ///< will be modified by library many times, drawn by library many times.
};


/// @brief Type of render primitive.
enum class PrimitiveType
{
     Points,        ///< separate points.

     Lines,         ///< separate lines.
     LineStrip,     ///< connected lines.
     LineLoop,      ///< connected lines, and the first vertex is connected with the last one.

     Triangles,     ///< separate triangles.
     TriangleStrip, ///< triangles connected with common edges.
     TriangleFan    ///< triangles connected with common vertex.
};


/// @brief Type of shader.
enum class ShaderType
{
     Vertex,        ///< shader operating on single vertices.
     Geometry,      ///< shader operating on sets of vertices of primitives.
     Fragment       ///< shader operating on primitives.
};


/// @brief Parameters for loading a resource.
/// @tparam T type of resource.
template < ResourceType T > struct LoadParams {};


/// @brief Parameters of texture loading.
template <>
struct LoadParams< ResourceType::Texture >
{
     BufferDataFormat format = BufferDataFormat::Rgb;       ///< format of texel data.
     TextureFilter    min_filter = TextureFilter::Linear;   ///< filter used when minifying the image.
     TextureFilter    mag_filter = TextureFilter::Linear;   ///< filter used when magnifying the image.
     TextureWrap      wrap_x = TextureWrap::ClampToEdge;    ///< wrap method for horizontal coordinate.
     TextureWrap      wrap_y = TextureWrap::ClampToEdge;    ///< wrap method for vertical coordinate.
     DataType         data_type = DataType::Byte;           ///< type of pixels data.
     Vec4f            border_color;                         ///< color of the border, for ClampToBorder wrapping only.
     Vec2i            size;                                 ///< size of texture in texels.
     const void*            data = nullptr;                 ///< data of a texture.
};


/// @brief Parameters of framebuffer loading.
template <>
struct LoadParams< ResourceType::FrameBuffer >
{
     /// @brief Parameters of loading a framebuffer attachment.
     struct AttachmentParams
     {
          StorageType      storage = StorageType::Texture2D;          ///< type of framebuffer attachment storage.
          AttachmentType   attachment = AttachmentType::Color;        ///< type of framebuffer attachment.
          BufferDataFormat format = BufferDataFormat::Rgb;            ///< format of texel data.
          TextureFilter    min_filter = TextureFilter::Linear;        ///< filter used when minifying the image.
          TextureFilter    mag_filter = TextureFilter::Linear;        ///< filter used when magnifying the image.
          TextureWrap      wrap_x = TextureWrap::ClampToEdge;         ///< wrap method for horizontal coordinate.
          TextureWrap      wrap_y = TextureWrap::ClampToEdge;         ///< wrap method for vertical coordinate.
          TextureWrap      wrap_z = TextureWrap::ClampToEdge;         ///< wrap method for z coordinate (for cubemaps only).
          DataType         data_type = DataType::Byte;                ///< type of pixels data.
          Vec4f            border_color;                              ///< color of the border, for ClampToBorder wrapping only.
          Vec2i            size;                                      ///< size of attachment in texels.
          std::size_t      samples = 0;                               ///< count of samples, 0 if not used.
          std::size_t      order = 0;                                 ///< order of attachment (for texture color attachments only).
     };

     std::vector< AttachmentParams > attachments;                     ///< parameters of attachments loading.
};


/// @brief Parameters of shader program loading.
template <>
struct LoadParams< ResourceType::Shader >
{
     /// @brief Parameters of single shader loading.
     struct ShaderParams
     {
          std::string entrypoint;                 ///< name of entry function.
          std::size_t size = 0;                   ///< size of shader data.
          const void *data = nullptr;             ///< data of shader.
          ShaderType type = ShaderType::Vertex;   ///< type of shader.
          bool from_source = false;               ///< compile shader from source code instead of loading binary.
     };
     std::vector< ShaderParams > shaders;    ///< parameters of all shaders of a shader program.
};


/// @brief Parameters of vertex buffer loading.
template <>
struct LoadParams< ResourceType::VertexBuffer >
{
     /// @brief Parameters of a buffer attribute.
     /// @details Attributes are always tightly packed.
     struct AttribParams
     {
          std::size_t size = Vec4f::size;         ///< size of an attribute (number of dimensions).
          DataType data_type = DataType::Float;   ///< type of buffer elements.
          bool normalized = false;                ///< should the data be normalized when loading.
     };

     /// @brief Parameters of a buffer.
     struct BufferParams
     {
          const void* data = nullptr;                  ///< data of the buffer.
          std::size_t size = 0;                        ///< size of the buffer, in bytes.
          BufferType type = BufferType::StaticDraw;    ///< type of memory used by buffer.
     };

     std::vector< AttribParams > attributes;      ///< parameters of attributes.
     BufferParams buffer;                         ///< buffer with data, interpreted with help of attributes.
     BufferParams index_buffer;                   ///< buffer with index data, contains array of indexes.
};


/// @brief Parameters of a render call.
struct RenderParams
{
     std::vector< Texture > textures;                  ///< textures to be used (binded) during the draw call.
     std::function< void( const IShader& ) > setup;    ///< function for setting uniforms,
                                                       ///< may be called in other thread and in other frame,
                                                       ///< so it must not capture context by reference.
     Shader shader;                                    ///< shader which will be used in render.
     FrameBuffer frame_buffer;                         ///< framebuffer for rendering (if ID is 0, will use default framebuffer).
     VertexBuffer vertex_buffer;                       ///< vertex buffer for rendering.
     PrimitiveType primitive = PrimitiveType::Points;  ///< type of primitive to draw.
     std::size_t vertex_count = 0;                     ///< number of vertices in each instance.
     std::size_t instance_count = 1;                   ///< number of instances to draw.
     bool clear_color = true;                          ///< should the color buffer be cleared.
     bool clear_depth = false;                         ///< should the depth buffer be cleared.
     bool clear_stencil = false;                       ///< should the stencil buffer be cleared.
};

} // namespace _16nar

#endif // #ifndef _16NAR_RENDER_DEFS_H
