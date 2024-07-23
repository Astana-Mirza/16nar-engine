#include <16nar/math/math_functions.h>

#include <glm/trigonometric.hpp>

namespace _16nar
{

float deg2rad( float degrees )
{
     return glm::radians( degrees );
}


float rad2deg( float radians )
{
     return glm::degrees( radians );
}

} // namespace _16nar
