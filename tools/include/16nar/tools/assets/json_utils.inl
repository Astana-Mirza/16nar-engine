#ifndef _16NAR_TOOLS_JSON_UTILS_INL
#define _16NAR_TOOLS_JSON_UTILS_INL

#include <nlohmann/json.hpp>
#include <16nar/render/render_defs.h>

namespace _16nar
{

NLOHMANN_JSON_SERIALIZE_ENUM( ResourceType, {
     { ResourceType::Unknown,      nullptr },
     { ResourceType::Texture,      "texture" },
     { ResourceType::Shader,       "shader" },
     { ResourceType::VertexBuffer, "vertex_buffer" },
     { ResourceType::Cubemap,      "cubemap" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( TextureWrap, {
     { TextureWrap::Repeat,             "repeat" },
     { TextureWrap::MirroredRepeat,     "mirrored_repeat" },
     { TextureWrap::ClampToEdge,        "clamp_to_edge" },
     { TextureWrap::ClampToBorder,      "clamp_to_border" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( TextureFilter, {
     { TextureFilter::Nearest,               "nearest" },
     { TextureFilter::Linear,                "linear" },
     { TextureFilter::NearestMipmapNearest,  "nearest_mipmap_nearest" },
     { TextureFilter::NearestMipmapLinear,   "nearest_mipmap_linear" },
     { TextureFilter::LinearMipmapNearest,   "linear_mipmap_nearest" },
     { TextureFilter::LinearMipmapLinear,    "linear_mipmap_linear" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( BufferDataFormat, {
     { BufferDataFormat::Rgb,           "rgb" },
     { BufferDataFormat::Rgba,          "rgba" },
     { BufferDataFormat::Srgb,          "srgb" },
     { BufferDataFormat::Srgba,         "srgba" }
} )


NLOHMANN_JSON_SERIALIZE_ENUM( DataType, {
     { DataType::Byte,    "byte" },
     { DataType::Float,   "float" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( ShaderType, {
     { ShaderType::Vertex,     "vertex" },
     { ShaderType::Fragment,   "fragment" },
     { ShaderType::Geometry,   "geometry" },
} )


NLOHMANN_JSON_SERIALIZE_ENUM( BufferType, {
     { BufferType::StreamDraw,  "stream_draw"  },
     { BufferType::StreamRead,  "stream_read"  },
     { BufferType::StreamCopy,  "stream_copy"  },
     { BufferType::StaticDraw,  "static_draw"  },
     { BufferType::StaticRead,  "static_read"  },
     { BufferType::StaticCopy,  "static_copy"  },
     { BufferType::DynamicDraw, "dynamic_draw" },
     { BufferType::DynamicRead, "dynamic_read" },
     { BufferType::DynamicCopy, "dynamic_copy" },
} )

} // namespace _16nar

#endif // #ifndef _16NAR_TOOLS_JSON_UTILS_INL
