/// @file
/// @brief Header file with ShaderProgram class definition.
#ifndef _16NAR_OPENGL_SHADER_PROGRAM_H
#define _16NAR_OPENGL_SHADER_PROGRAM_H

#include <16nar/render/ishader_program.h>

namespace _16nar::opengl
{

/// @brief Implementation of shader program interface for OpenGL.
class ShaderProgram : public IShaderProgram
{
public:
     /// @brief Constructor.
     /// @param[in] descriptor descriptor of compiled and linked shader program.
     ShaderProgram( unsigned int descriptor );

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, float value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, int)
     virtual void set_uniform( std::string_view name, int value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, bool)
     virtual void set_uniform( std::string_view name, bool value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec2i&)
     virtual void set_uniform( std::string_view name, const Vec2i& value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec2f&)
     virtual void set_uniform( std::string_view name, const Vec2f& value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec3i&)
     virtual void set_uniform( std::string_view name, const Vec3i& value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec3f&)
     virtual void set_uniform( std::string_view name, const Vec3f& value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec4i&)
     virtual void set_uniform( std::string_view name, const Vec4i& value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec4f&)
     virtual void set_uniform( std::string_view name, const Vec4f& value ) const override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const TransformMatrix&)
     virtual void set_uniform( std::string_view name, const TransformMatrix& value ) const override;

private:
     unsigned int descriptor_;          ///< descriptor of shader program.
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_SHADER_PROGRAM_H
