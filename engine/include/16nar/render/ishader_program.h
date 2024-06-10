/// @file
/// @brief Header file with IShaderProgram interface definition.
#ifndef _16NAR_ISHADER_PROGRAM_H
#define _16NAR_ISHADER_PROGRAM_H

#include <16nar/16nardefs.h>

#include <16nar/math/vec.h>

#include <string_view>

namespace _16nar
{

/// @brief Interface for shader programs.
/// @details This interface should be used to load various parameters to
/// shader programs. Compilation and linking of shader program is out of scope of
/// this interface. Instead, graphics API-specific loaders should be used.
class ENGINE_API IShaderProgram
{
public:
     /// @brief Default virtual destructor.
     virtual ~IShaderProgram() = default;

     /// @brief Set value of the uniform.
     /// @param[in] name name of the uniform.
     /// @param[in] value value of the uniform.
     virtual void set_uniform( std::string_view name, float value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, int value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, bool value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, const Vec2i& value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, const Vec2f& value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, const Vec3i& value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, const Vec3f& value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, const Vec4i& value ) const = 0;

     /// @copydoc IShaderProgram::set_uniform(std::string_view, float)
     virtual void set_uniform( std::string_view name, const Vec4f& value ) const = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_ISHADER_PROGRAM_H
