#include <16nar/constructor/render/drawable.h>

namespace _16nar::constructor
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

} // namespace _16nar::constructor
