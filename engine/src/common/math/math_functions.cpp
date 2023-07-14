#include <16nar/common/math/math_functions.h>

#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
#    include <glm/gtx/transform.hpp>
#endif

namespace _16nar
{

float deg2rad( float degrees )
{
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
     return glm::radians( degrees );
#endif
}


float rad2deg( float radians )
{
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
     return glm::degrees( radians );
#endif
}

} // namespace _16nar
