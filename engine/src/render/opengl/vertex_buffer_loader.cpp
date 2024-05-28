#include <16nar/render/opengl/vertex_buffer_loader.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

#include <glad/gl.h>

namespace _16nar::opengl
{

bool VertexBufferLoader::load( const LoadParams& params, Handler& handler )
{
     LOG_16NAR_DEBUG( "Vertex buffer " << handler.vao_descriptor << " was loaded" );
     return true;
}


bool VertexBufferLoader::unload( const Handler& handler )
{
     LOG_16NAR_DEBUG( "Vertex buffer " << handler.vao_descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
