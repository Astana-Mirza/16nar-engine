#include <16nar/render/opengl/framebuffer_loader.h>

#include <glad/gl.h>

namespace _16nar::opengl
{

bool FrameBufferLoader::unload( const Handler& handler )
{
     return true;
}


FrameBufferLoader::FrameBufferLoader( const LoadParams& params ):
     params_{ params }, handler_{}
{}


bool FrameBufferLoader::load_impl() const
{
     return true;
}

} // namespace _16nar::opengl
