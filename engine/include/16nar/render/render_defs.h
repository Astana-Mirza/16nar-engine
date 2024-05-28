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


/// @brief Type of attachment to OpenGL framebuffer.
enum class AttachmentType
{
     RenderBuffer,       ///< renderbuffer, cannot be read from.
     Texture2D,          ///< simple 2D texture.
     Texture3D           ///< cubemap.
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
     TriangleFan,   ///< triangles connected with common vertex.
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
     TextureWrap      wrap_z = TextureWrap::ClampToEdge;    ///< wrap method for z coordinate (for cubemaps only).
     DataType         data_type = DataType::Byte;           ///< type of pixels data.
     Vec4f            border_color;                         ///< color of the border, for ClampToBorder wrapping only.
     Vec2i            size;                                 ///< size of texture in texels.
     std::size_t      samples = 0;                          ///< count of samples, 0 if not used.
     void*            data = nullptr;                       ///< data of a texture.
};


/// @brief Parameters of framebuffer loading.
template <>
struct LoadParams< ResourceType::FrameBuffer >
{
     /// @brief Parameters of loading a framebuffer attachment.
     struct AttachmentParams
     {
          AttachmentType   attachment = AttachmentType::Texture2D;    ///< type of framebuffer attachment.
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
     };

     std::vector< AttachmentParams > attachments;                     ///< parameters of attachments loading.
};


/// @brief Parameters of shader loading.
template <>
struct LoadParams< ResourceType::Shader >
{
     std::string entrypoint;       ///< name of entry function.
     void *data;                   ///< data of the shader.
};


/// @brief Parameters of vertex buffer loading.
template <>
struct LoadParams< ResourceType::VertexBuffer >
{
     /// @brief Parameters of a buffer attribute.
     struct AttribParams
     {
          std::size_t size = Vec4f::size;         ///< size of an attribute (number of dimensions).
          DataType data_type = DataType::Float;   ///< type of buffer elements.
          bool tightly_packed = false;            ///< is the attribute data tightly packed.
          bool normalized = false;                ///< should the data be normalized when loading.
     };

     /// @brief Parameters of a buffer.
     struct BufferParams
     {
          void* data = nullptr;                        ///< data of the buffer.
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
     FrameBuffer frame_buffer;                         ///< framebuffer for rendering.
     VertexBuffer vertex_buffer;                       ///< vertex buffer for rendering.
     PrimitiveType primitive;                          ///< type of primitive to draw.
};

} // namespace _16nar

#endif // #ifndef _16NAR_RENDER_DEFS_H
