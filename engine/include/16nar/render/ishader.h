/// @file
/// @brief Header file with IShader interface definition.
#ifndef _16NAR_ISHADER_H
#define _16NAR_ISHADER_H

#include <16nar/16nardefs.h>

namespace _16nar
{

/// @brief Interface for shader programs.
class ENGINE_API IShader
{
public:
     /// @brief Default virtual destructor.
     virtual ~IShader() = default;

     /// @brief Set value of the uniform.
     /// @param[in] value value of the uniform.
     virtual void set_uniform( float value ) const = 0;

     /// @copydoc IShader::set_uniform(float)
     virtual void set_uniform( int value ) const = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_ISHADER_H
