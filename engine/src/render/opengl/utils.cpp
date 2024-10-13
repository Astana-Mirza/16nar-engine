#include <16nar/render/opengl/utils.h>

#include <16nar/render/render_defs.h>
#include <16nar/render/opengl/glad.h>

#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

unsigned int tex_wrap_to_int( TextureWrap wrap )
{
     switch ( wrap )
     {
          case TextureWrap::Repeat:          return GL_REPEAT;
          case TextureWrap::MirroredRepeat:  return GL_MIRRORED_REPEAT;
          case TextureWrap::ClampToEdge:     return GL_CLAMP_TO_EDGE;
          case TextureWrap::ClampToBorder:   return GL_CLAMP_TO_BORDER;
     }
     LOG_16NAR_ERROR( "Wrong texture wrap parameter: "
          << static_cast< std::size_t >( wrap ) << ", will use GL_REPEAT" );
     return GL_REPEAT;
}


unsigned int tex_filter_to_int( TextureFilter filter )
{
     switch ( filter )
     {
          case TextureFilter::Nearest:                 return GL_NEAREST;
          case TextureFilter::Linear:                  return GL_LINEAR;
          case TextureFilter::NearestMipmapNearest:    return GL_NEAREST_MIPMAP_NEAREST;
          case TextureFilter::NearestMipmapLinear:     return GL_NEAREST_MIPMAP_LINEAR;
          case TextureFilter::LinearMipmapNearest:     return GL_LINEAR_MIPMAP_NEAREST;
          case TextureFilter::LinearMipmapLinear:      return GL_LINEAR_MIPMAP_LINEAR;
     }
     LOG_16NAR_ERROR( "Wrong texture filter parameter: "
          << static_cast< std::size_t >( filter ) << ", will use GL_NEAREST" );
     return GL_NEAREST;
}


unsigned int data_format_to_int( BufferDataFormat format )
{
     switch ( format )
     {
          case BufferDataFormat::Rgb:             return GL_RGB;
          case BufferDataFormat::Rgba:            return GL_RGBA;
          case BufferDataFormat::Srgb:            return GL_SRGB;
          case BufferDataFormat::Srgba:           return GL_SRGB_ALPHA;
          case BufferDataFormat::Depth:           return GL_DEPTH_COMPONENT;
          case BufferDataFormat::DepthStencil:    return GL_DEPTH_STENCIL;
     }
     LOG_16NAR_ERROR( "Wrong buffer data format parameter: "
          << static_cast< std::size_t >( format ) << ", will use GL_RGB" );
     return GL_RGB;
}


unsigned int data_type_to_int( DataType type )
{
     switch ( type )
     {
          case DataType::Byte:     return GL_UNSIGNED_BYTE;
          case DataType::Float:    return GL_FLOAT;
     }
     LOG_16NAR_ERROR( "Wrong buffer data type parameter: "
          << static_cast< std::size_t >( type ) << ", will use GL_BYTE" );
     return GL_BYTE;
}


unsigned int buffer_type_to_int( BufferType type )
{
     switch ( type )
     {
          case BufferType::StreamDraw:       return GL_STREAM_DRAW;
          case BufferType::StreamRead:       return GL_STREAM_READ;
          case BufferType::StreamCopy:       return GL_STREAM_COPY;
          case BufferType::StaticDraw:       return GL_STATIC_DRAW;
          case BufferType::StaticRead:       return GL_STATIC_READ;
          case BufferType::StaticCopy:       return GL_STATIC_COPY;
          case BufferType::DynamicDraw:      return GL_DYNAMIC_DRAW;
          case BufferType::DynamicRead:      return GL_DYNAMIC_READ;
          case BufferType::DynamicCopy:      return GL_DYNAMIC_COPY;
     }
     LOG_16NAR_ERROR( "Wrong buffer type parameter: "
          << static_cast< std::size_t >( type ) << ", will use GL_STREAM_DRAW" );
     return GL_STREAM_DRAW;
}


unsigned int primitive_type_to_int( PrimitiveType type )
{
     switch ( type )
     {
          case PrimitiveType::Points:        return GL_POINTS;
          case PrimitiveType::Lines:         return GL_LINES;
          case PrimitiveType::LineStrip:     return GL_LINE_STRIP;
          case PrimitiveType::LineLoop:      return GL_LINE_LOOP;
          case PrimitiveType::Triangles:     return GL_TRIANGLES;
          case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
          case PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
     }
     LOG_16NAR_ERROR( "Wrong primitive type parameter: "
          << static_cast< std::size_t >( type ) << ", will use GL_POINTS" );
     return GL_POINTS;
}


unsigned int default_internal_format( BufferDataFormat format, DataType data_type )
{
     switch ( format )
     {
          case BufferDataFormat::Rgb:             return data_type == DataType::Byte ? GL_RGB8 : GL_RGB16F;
          case BufferDataFormat::Rgba:            return data_type == DataType::Byte ? GL_RGBA8 : GL_RGBA16F;
          case BufferDataFormat::Srgb:            return data_type == DataType::Byte ? GL_SRGB8 : GL_RGB16F;
          case BufferDataFormat::Srgba:           return data_type == DataType::Byte ? GL_SRGB8_ALPHA8 : GL_RGBA16F;
          case BufferDataFormat::Depth:           return data_type == DataType::Byte ? GL_DEPTH_COMPONENT16 : GL_DEPTH_COMPONENT32F;
          case BufferDataFormat::DepthStencil:    return data_type == DataType::Byte ? GL_DEPTH24_STENCIL8 : GL_DEPTH32F_STENCIL8;
     }
     LOG_16NAR_ERROR( "Wrong buffer data format parameter: "
          << static_cast< std::size_t >( format ) << ", unable to get internal format, will use GL_RGB8" );
     return GL_RGB8;
}


unsigned int attachment_type_to_int(  AttachmentType type, std::size_t order  )
{
     switch ( type )
     {
          case AttachmentType::Color:             return GL_COLOR_ATTACHMENT0 + order;
          case AttachmentType::Depth:             return GL_DEPTH_ATTACHMENT;
          case AttachmentType::DepthStencil:      return GL_DEPTH_STENCIL_ATTACHMENT;
     }
     LOG_16NAR_ERROR( "Wrong attachment type parameter: "
          << static_cast< std::size_t >( type ) << ", will use GL_COLOR_ATTACHMENT0" );
     return GL_COLOR_ATTACHMENT0;
}


unsigned int shader_type_to_int( ShaderType type )
{
     switch ( type )
     {
          case ShaderType::Vertex:      return GL_VERTEX_SHADER;
          case ShaderType::Geometry:    return GL_GEOMETRY_SHADER;
          case ShaderType::Fragment:    return GL_FRAGMENT_SHADER;
     }
     LOG_16NAR_ERROR( "Wrong shader type parameter: "
          << static_cast< std::size_t >( type ) << ", will use GL_VERTEX_SHADER" );
     return GL_VERTEX_SHADER;
}

} // namespace _16nar::opengl
