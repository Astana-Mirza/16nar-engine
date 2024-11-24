#include <16nar/render/drawable.h>

namespace _16nar
{

bool Drawable::is_visible() const noexcept
{
     return visible_;
}


void Drawable::set_visible( bool visible ) noexcept
{
     visible_ = visible;
}


const Shader& Drawable::get_shader() const noexcept
{
     return shader_;
}

} // namespace _16nar
