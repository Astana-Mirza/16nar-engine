#include <16nar/render/opengl/shader_loader.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

#include <glad/gl.h>

namespace _16nar::opengl
{

bool ShaderLoader::load( const LoadParams& params, Handler& handler )
{
     LOG_16NAR_DEBUG( "Shader " << handler.descriptor << " was loaded" );
     return true;
}


bool ShaderLoader::unload( const Handler& handler )
{
     LOG_16NAR_DEBUG( "Shader " << handler.descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
