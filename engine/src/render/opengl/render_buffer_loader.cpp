#include <16nar/render/opengl/render_buffer_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

bool RenderBufferLoader::load( const ResourceManagerMap&,
     const LoadParamsType& params, HandlerType& handler )
{
     glGenRenderbuffers( 1, &handler.descriptor );
     glBindRenderbuffer( GL_RENDERBUFFER, handler.descriptor );
     if ( params.samples > 0 )
     {
          glRenderbufferStorageMultisample( GL_RENDERBUFFER, params.samples,
               default_internal_format( params.format, params.data_type ), params.size.x(), params.size.y() );
     }
     else
     {
          glRenderbufferStorage( GL_RENDERBUFFER,
               default_internal_format( params.format, params.data_type ),
               params.size.x(), params.size.y() );
     }
     glBindRenderbuffer( GL_RENDERBUFFER, 0 );
     LOG_16NAR_DEBUG( "Render buffer " << handler.descriptor << " was loaded" );
     return true;
}


bool RenderBufferLoader::unload( const HandlerType& handler )
{
     glDeleteRenderbuffers( 1, &handler.descriptor );
     LOG_16NAR_DEBUG( "Render buffer " << handler.descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
