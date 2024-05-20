#include <16nar/render/opengl/texture_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/gl.h>

#include <16nar/system/exceptions.h>

namespace _16nar::opengl
{

bool TextureLoader::unload( TextureLoader::Handler handler )
{
     return true;
}


TextureLoader::TextureLoader( const LoadParams& params ):
     params_{ params }, handler_{}
{}


bool TextureLoader::load_impl() const
{
     
     return true;
}


TextureLoader load_texture( const TextureLoader::LoadParams& params, std::string_view file )
{
     int channels = 0;
     int req_channels = 0;
     int x = params.size.x(), y = params.size.y();
     LoadParams load_params = params;
     if ( params.format == BufferDataFormat::Rgb || params.format == BufferDataFormat::Srgb )
     {
          req_channels = 3;
     }
     else if ( params.format == BufferDataFormat::Rgba || params.format == BufferDataFormat::Srgba )
     {
          req_channels = 4;
     }
     if ( params.data_type == DataType::Byte )
     {
          load_params.data = ::stbi_load( file.data(), &x, &y, &channels, req_channels );
          return TextureLoader{ load_params };
     }
     else if ( params.data_type == DataType::Float )
     {
          load_params.data = ::stbi_loadf( file.data(), &x, &y, &channels, req_channels );
          return TextureLoader{ load_params };
     }
     throw ResourceException{ "unable to load texture from file, wrong data type" };
}

} // namespace _16nar::opengl
