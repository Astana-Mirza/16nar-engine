#include <16nar/render/drawable.h>

namespace _16nar
{

bool Drawable::is_visible() const
{
     return visible_;
}


void Drawable::set_visible( bool visible )
{
     visible_ = visible;
}


const Shader& Drawable::get_shader() const
{
     return shader_;
}

} // namespace _16nar
