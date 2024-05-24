#include <16nar/render/opengl/framebuffer_loader.h>

#include <16nar/logger/logger.h>

#include <glad/gl.h>

namespace _16nar::opengl
{

bool FrameBufferLoader::unload( const Handler& handler )
{
     LOG_16NAR_DEBUG( "Frame buffer " << handler.descriptor << " was unloaded" );
     return true;
}


FrameBufferLoader::FrameBufferLoader( const LoadParams& params ):
     params_{ params }, handler_{}
{}


bool FrameBufferLoader::load_impl() const
{
     LOG_16NAR_DEBUG( "Frame buffer " << handler_.descriptor << " was loaded" );
     return true;
}

} // namespace _16nar::opengl
