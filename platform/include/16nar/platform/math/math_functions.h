/// @file Header file with useful mathematical functions declarations.
#ifndef _16NAR_PLATFORM_MATH_MATH_FUNCTIONS_H
#define _16NAR_PLATFORM_MATH_MATH_FUNCTIONS_H

#include <16nar/platform/defs.h>

#include <16nar/platform/math/vec.h>

#include <glm/trigonometric.hpp>

namespace _16nar::math
{

/// @brief Convert degrees to radians.
/// @param[in] degrees degrees to be converted to radians.
/// @return value in radians.
inline float deg2rad( float degrees ) noexcept
{
     return glm::radians( degrees );
}


/// @brief Convert radians to degrees.
/// @param[in] radians radians to be converted to degrees.
/// @return value in degrees.
inline float rad2deg( float radians ) noexcept
{
     return glm::degrees( radians );
}

} // namespace _16nar::math

#endif // #ifndef _16NAR_PLATFORM_MATH_MATH_FUNCTIONS_H
