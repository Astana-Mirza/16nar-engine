#include <16nar/render/opengl/texture_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

bool TextureLoader::load( const ResourceManagerMap&, const LoadParams& params, Handler& handler )
{
     unsigned int texture_type = 0;
     glGenTextures( 1, &handler.descriptor );

     if ( params.samples > 0 )
     {
          texture_type = GL_TEXTURE_2D_MULTISAMPLE;
          glBindTexture( texture_type, handler.descriptor );
          glTexImage2DMultisample( texture_type, params.samples, data_format_to_int( params.format ),
               params.size.x(), params.size.y(), GL_TRUE );
     }
     else
     {
          texture_type = GL_TEXTURE_2D;
          glBindTexture( texture_type, handler.descriptor );
          glTexImage2D( texture_type, 0, data_format_to_int( params.format ), params.size.x(), params.size.y(),
               0, data_format_to_int( params.format ), data_type_to_int( params.data_type ), params.data.get() );
     }
     glTexParameteri( texture_type, GL_TEXTURE_MIN_FILTER, tex_filter_to_int( params.min_filter ) );
     glTexParameteri( texture_type, GL_TEXTURE_MAG_FILTER, tex_filter_to_int( params.mag_filter ) );
     glTexParameteri( texture_type, GL_TEXTURE_WRAP_S, tex_wrap_to_int( params.wrap_x ) );
     glTexParameteri( texture_type, GL_TEXTURE_WRAP_T, tex_wrap_to_int( params.wrap_y ) );
     glTexParameterfv( texture_type, GL_TEXTURE_BORDER_COLOR, params.border_color.data() );
     glBindTexture( texture_type, 0 );
     LOG_16NAR_DEBUG( "Texture " << handler.descriptor << " was loaded" );
     return true;
}


bool TextureLoader::unload( const Handler& handler )
{
     glDeleteTextures( 1, &handler.descriptor );
     LOG_16NAR_DEBUG( "Texture " << handler.descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
