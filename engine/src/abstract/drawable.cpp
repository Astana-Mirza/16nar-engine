#include <abstract/drawable.h>

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


int Drawable::get_layer() const
{
     return layer_;
}


void Drawable::set_layer( int layer )
{
     layer_ = layer;
}

} // namespace _16nar
