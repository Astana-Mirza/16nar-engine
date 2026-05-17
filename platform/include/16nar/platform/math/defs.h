/// @file
/// @brief File with basic definitions for the math component.
#ifndef _16NAR_PLATFORM_MATH_DEFS_H
#define _16NAR_PLATFORM_MATH_DEFS_H

#include <16nar/platform/defs.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_INLINE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_SIZE_T_LENGTH

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_float2x3.hpp>
#include <glm/ext/matrix_float2x4.hpp>
#include <glm/ext/matrix_float3x2.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float3x4.hpp>
#include <glm/ext/matrix_float4x2.hpp>
#include <glm/ext/matrix_float4x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// need #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtc/quaternion.hpp> 
// #include <glm/gtx/norm.hpp>

namespace _16nar::math
{

using Vec2f = glm::vec2;
using Vec3f = glm::vec3;
using Vec4f = glm::vec4;

using Vec2i = glm::ivec2;
using Vec3i = glm::ivec3;
using Vec4i = glm::ivec4;

using Mat2x2f = glm::mat2;
using Mat2x3f = glm::mat2x3;
using Mat2x4f = glm::mat2x4;

using Mat3x2f = glm::mat3x2;
using Mat3x3f = glm::mat3;
using Mat3x4f = glm::mat3x4;

using Mat4x2f = glm::mat4x2;
using Mat4x4f = glm::mat4;
using Mat4x3f = glm::mat4x3;

// using Quatf = glm::quat;

using glm::clamp;
using glm::mix;
using glm::smoothstep;
using glm::radians;
using glm::degrees;
using glm::sin;
using glm::cos;
using glm::tan;
using glm::asin;
using glm::acos;
using glm::atan;
using glm::sinh;
using glm::cosh;
using glm::tanh;
using glm::asinh;
using glm::acosh;
using glm::atanh;

using glm::length;
// using glm::length2;
using glm::distance;
// using glm::distance2;
using glm::normalize;
using glm::dot;
using glm::cross;

using glm::translate;
using glm::rotate;
using glm::scale;
using glm::perspective;
using glm::ortho;

// using glm::slerp;
// using glm::mat4_cast;


template< typename... Args >
inline auto look_at( Args&&... args )
{
     return glm::lookAt( std::forward< Args >( args )... );
}


/// @brief Integer rectangle.
struct IntRect
{
     Vec2i pos{};   ///< top left position of rectangle.
     Vec2i size{};  ///< size of rectangle.
};


/// @brief Float rectangle.
struct FloatRect
{
     Vec2f pos{};   ///< top left position of rectangle.
     Vec2f size{};  ///< size of rectangle.
};

} // namespace _16nar::math

#endif // #ifndef _16NAR_PLATFORM_MATH_DEFS_H
