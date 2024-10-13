#include <16nar/render/opengl/cubemap_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

bool CubemapLoader::load( const ResourceManagerMap&, const LoadParamsType& params, HandlerType& handler )
{
     glGenTextures( 1, &handler.descriptor );
     glBindTexture( GL_TEXTURE_CUBE_MAP, handler.descriptor );
     for ( unsigned int i = 0; i < params.data.size(); ++i )
     {
          glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
               data_format_to_int( params.format ), params.size.x(), params.size.y(),
               0, data_format_to_int( params.format ),
               data_type_to_int( params.data_type ), params.data[ i ].get() );
     }
     glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, tex_filter_to_int( params.min_filter ) );
     glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, tex_filter_to_int( params.mag_filter ) );
     glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tex_wrap_to_int( params.wrap_x ) );
     glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tex_wrap_to_int( params.wrap_y ) );
     glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, tex_wrap_to_int( params.wrap_z ) );
     glTexParameterfv( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, params.border_color.data() );
     glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
     LOG_16NAR_DEBUG( "Cubemap " << handler.descriptor << " was loaded" );
     return true;
}
 

bool CubemapLoader::unload( const HandlerType& handler )
{
     glDeleteTextures( 1, &handler.descriptor );
     LOG_16NAR_DEBUG( "Cubemap " << handler.descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
