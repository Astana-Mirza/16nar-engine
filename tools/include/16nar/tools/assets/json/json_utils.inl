#ifndef _16NAR_TOOLS_JSON_UTILS_INL
#define _16NAR_TOOLS_JSON_UTILS_INL

#include <nlohmann/json.hpp>
#include <16nar/tools/scene_defs.h>
#include <16nar/render/render_defs.h>

namespace _16nar
{

NLOHMANN_JSON_SERIALIZE_ENUM( ResourceType, {
     { ResourceType::Unknown,      nullptr },
     { ResourceType::Texture,      "texture" },
     { ResourceType::Shader,       "shader" },
     { ResourceType::VertexBuffer, "vertex_buffer" },
     { ResourceType::Cubemap,      "cubemap" },
     { ResourceType::DataSchema,   "data_schema" },
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


namespace tools
{

NLOHMANN_JSON_SERIALIZE_ENUM( StoredDataType, {
     { StoredDataType::Unknown,         nullptr             },
     { StoredDataType::Uint64,          "uint64"            },
     { StoredDataType::Uint32,          "uint32"            },
     { StoredDataType::Uint16,          "uint16"            },
     { StoredDataType::Uint8,           "uint8"             },
     { StoredDataType::Int64,           "int64"             },
     { StoredDataType::Int32,           "int32"             },
     { StoredDataType::Int16,           "int16"             },
     { StoredDataType::Int8,            "int8"              },
     { StoredDataType::Bool,            "bool"              },
     { StoredDataType::Float,           "float"             },
     { StoredDataType::Double,          "double"            },
     { StoredDataType::String,          "string"            },

     { StoredDataType::Uint64Arr,       "uint64_arr"        },
     { StoredDataType::Uint32Arr,       "uint32_arr"        },
     { StoredDataType::Uint16Arr,       "uint16_arr"        },
     { StoredDataType::Uint8Arr,        "uint8_arr"         },
     { StoredDataType::Int64Arr,        "int64_arr"         },
     { StoredDataType::Int32Arr,        "int32_arr"         },
     { StoredDataType::Int16Arr,        "int16_arr"         },
     { StoredDataType::Int8Arr,         "int8_arr"          },
     { StoredDataType::BoolArr,         "bool_arr"          },
     { StoredDataType::FloatArr,        "float_arr"         },
     { StoredDataType::FloatArr,        "double_arr"        },
     { StoredDataType::StringArr,       "string_arr"        },

     { StoredDataType::Vec2f,           "vec2f"             },
     { StoredDataType::Vec3f,           "vec3f"             },
     { StoredDataType::Vec4f,           "vec4f"             },
     { StoredDataType::Vec2i,           "vec2i"             },
     { StoredDataType::Vec3i,           "vec3i"             },
     { StoredDataType::Vec4i,           "vec4i"             },
     { StoredDataType::FloatRect,       "float_rect"        },
     { StoredDataType::IntRect,         "int_rect"          },
     { StoredDataType::ResourceIndex,   "resource_index"    },
} )

} // namespace tools

} // namespace _16nar

#endif // #ifndef _16NAR_TOOLS_JSON_UTILS_INL
