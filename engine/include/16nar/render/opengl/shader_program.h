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
     ShaderProgram( unsigned int descriptor ) noexcept;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float) const noexcept
     virtual void set_uniform( std::string_view name, float value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, int) const noexcept
     virtual void set_uniform( std::string_view name, int value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, bool) const noexcept
     virtual void set_uniform( std::string_view name, bool value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec2i&) const noexcept
     virtual void set_uniform( std::string_view name, const Vec2i& value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec2f&) const noexcept
     virtual void set_uniform( std::string_view name, const Vec2f& value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec3i&) const noexcept
     virtual void set_uniform( std::string_view name, const Vec3i& value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec3f&) const noexcept
     virtual void set_uniform( std::string_view name, const Vec3f& value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec4i&) const noexcept
     virtual void set_uniform( std::string_view name, const Vec4i& value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const Vec4f&) const noexcept
     virtual void set_uniform( std::string_view name, const Vec4f& value ) const noexcept override;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, const TransformMatrix&) const noexcept
     virtual void set_uniform( std::string_view name, const TransformMatrix& value ) const noexcept override;

private:
     unsigned int descriptor_;          ///< descriptor of shader program.
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_SHADER_PROGRAM_H
