#include <16nar/render/opengl/shader_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{
namespace
{

void log_compile_error( unsigned int descriptor )
{
     GLint max_length = 0;
     glGetShaderiv( descriptor, GL_INFO_LOG_LENGTH, &max_length );
     std::vector< GLchar > err_msg( max_length );
     glGetShaderInfoLog( descriptor, max_length, &max_length, &err_msg[ 0 ] );
     LOG_16NAR_ERROR( "Unable to load shader program: " << err_msg.data() );
}


void log_link_error( unsigned int descriptor )
{
     GLint max_length = 0;
     glGetProgramiv( descriptor, GL_INFO_LOG_LENGTH, &max_length );
     std::vector< GLchar > err_msg( max_length );
     glGetProgramInfoLog( descriptor, max_length, &max_length, &err_msg[ 0 ] );
     LOG_16NAR_ERROR( "Unable to load shader program: " << err_msg.data() );
}


bool load_shader_from_source( const LoadParams< ResourceType::Shader >::ShaderParams& params,
     unsigned int& descriptor )
{
     const char *source = reinterpret_cast< const char * >( params.data );
     descriptor = glCreateShader( shader_type_to_int( params.type ) );
     glShaderSource( descriptor, 1, &source, nullptr );
     glCompileShader( descriptor );

     GLint is_compiled = 0;
     glGetShaderiv( descriptor, GL_COMPILE_STATUS, &is_compiled );
     if ( is_compiled == GL_FALSE )
     {
          log_compile_error( descriptor );
          glDeleteShader( descriptor );
          descriptor = 0;
          return false;
     }
     return true;
}


bool load_shader( const LoadParams< ResourceType::Shader >::ShaderParams& params, unsigned int& descriptor )
{
     descriptor = glCreateShader( shader_type_to_int( params.type ) );
     glShaderBinary( 1, &descriptor, GL_SHADER_BINARY_FORMAT_SPIR_V, params.data, params.size );
     glSpecializeShader( descriptor, params.entrypoint.c_str(), 0, nullptr, nullptr );

     GLint is_compiled = 0;
     glGetShaderiv( descriptor, GL_COMPILE_STATUS, &is_compiled );
     if ( is_compiled == GL_FALSE )
     {
          log_compile_error( descriptor );
          glDeleteShader( descriptor );
          descriptor = 0;
          return false;
     }
     return true;
}


void unload_shaders( const std::vector< unsigned int >& shaders, unsigned int program )
{
     for ( auto descriptor : shaders )
     {
          glDeleteShader( descriptor );
          if ( program != 0 )
          {
               glDetachShader( program, descriptor );
          }
     }
}

} // anonymous namespace


bool ShaderLoader::load( const ResourceManagerMap&, const LoadParams& params, Handler& handler )
{
     GLint is_linked = 0;
     std::vector< unsigned int > descriptors;
     descriptors.reserve( params.shaders.size() );

     for ( const auto& shader : params.shaders )
     {
          unsigned int descriptor = 0;
          if ( ( shader.from_source ? load_shader_from_source( shader, descriptor )
               : load_shader( shader, descriptor ) ) )
          {
               descriptors.push_back( descriptor );
          }
          else
          {
               unload_shaders( descriptors, 0 );
               return false;
          }
     }

     handler.descriptor = glCreateProgram();
     for ( auto descriptor : descriptors )
     {
          glAttachShader( handler.descriptor, descriptor );
     }
     glLinkProgram( handler.descriptor );
     glGetProgramiv( handler.descriptor, GL_LINK_STATUS, &is_linked );

     unload_shaders( descriptors, handler.descriptor );
     if ( is_linked == GL_FALSE )
     {
          log_link_error( handler.descriptor );
          glDeleteProgram( handler.descriptor );
          handler.descriptor = 0;
          return false;
     }

     LOG_16NAR_DEBUG( "Shader " << handler.descriptor << " was loaded" );
     return true;
}


bool ShaderLoader::unload( const Handler& handler )
{
     glDeleteProgram( handler.descriptor );
     LOG_16NAR_DEBUG( "Shader " << handler.descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
