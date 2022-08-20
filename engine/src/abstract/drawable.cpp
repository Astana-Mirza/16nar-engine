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


void Drawable::set_shader( Shader *shader )
{
     shader_ = shader;
}


Shader *Drawable::get_shader() const
{
     return shader_;
}


void Drawable::set_blend( const BlendMode& blend )
{
     blend_ = blend;
}


const BlendMode& Drawable::get_blend() const
{
     return blend_;
}

} // namespace _16nar
