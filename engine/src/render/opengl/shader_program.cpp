#include <16nar/render/opengl/shader_program.h>

#include <16nar/render/opengl/glad.h>
#include <16nar/math/transform_matrix.h>

namespace _16nar::opengl
{

ShaderProgram::ShaderProgram( unsigned int descriptor ):
     descriptor_{ descriptor }
{}


void ShaderProgram::set_uniform( std::string_view name, float value ) const
{
     glUniform1f( glGetUniformLocation( descriptor_, name.data() ), value );
}


void ShaderProgram::set_uniform( std::string_view name, int value ) const
{
     glUniform1i( glGetUniformLocation( descriptor_, name.data() ), value );
}


void ShaderProgram::set_uniform( std::string_view name, bool value ) const
{
     glUniform1i( glGetUniformLocation( descriptor_, name.data() ), static_cast< int >( value ) );
}


void ShaderProgram::set_uniform( std::string_view name, const Vec2i& value ) const
{
     glUniform2i( glGetUniformLocation( descriptor_, name.data() ), value.x(), value.y() );
}


void ShaderProgram::set_uniform( std::string_view name, const Vec2f& value ) const
{
     glUniform2f( glGetUniformLocation( descriptor_, name.data() ), value.x(), value.y() );
}


void ShaderProgram::set_uniform( std::string_view name, const Vec3i& value ) const
{
     glUniform3i( glGetUniformLocation( descriptor_, name.data() ), value.x(), value.y(), value.z() );
}


void ShaderProgram::set_uniform( std::string_view name, const Vec3f& value ) const
{
     glUniform3f( glGetUniformLocation( descriptor_, name.data() ), value.x(), value.y(), value.z() );
}


void ShaderProgram::set_uniform( std::string_view name, const Vec4i& value ) const
{
     glUniform4i( glGetUniformLocation( descriptor_, name.data() ), value.x(), value.y(), value.z(), value.w() );
}


void ShaderProgram::set_uniform( std::string_view name, const Vec4f& value ) const
{
     glUniform4f( glGetUniformLocation( descriptor_, name.data() ), value.x(), value.y(), value.z(), value.w() );
}


void ShaderProgram::set_uniform( std::string_view name, const TransformMatrix& value ) const
{
     glUniformMatrix4fv( glGetUniformLocation( descriptor_, name.data() ), 1, GL_FALSE, value.data() );
}

} // namespace _16nar::opengl
