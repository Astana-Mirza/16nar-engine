/// @file Header file with useful mathematical functions declarations.
#ifndef _16NAR_MATH_FUNCTIONS_H
#define _16NAR_MATH_FUNCTIONS_H

#include <16nar/16nardefs.h>
#include <cmath>

namespace _16nar
{

/// @brief Convert degrees to radians.
/// @param[in] degrees degrees to be converted to radians.
/// @return value in radians.
ENGINE_API float deg2rad( float degrees ) noexcept;


/// @brief Convert radians to degrees.
/// @param[in] radians radians to be converted to degrees.
/// @return value in degrees.
ENGINE_API float rad2deg( float radians ) noexcept;

} // namespace _16nar

#endif // #ifndef _16NAR_MATH_FUNCTIONS_H
