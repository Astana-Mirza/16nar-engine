#include <16nar/constructor/render/drawable_2d.h>

#include <16nar/constructor/render/irender_system_2d.h>

namespace _16nar::constructor
{

Drawable2D::Drawable2D( const Shader& shader, IRenderSystem2D *render_system ):
     render_system_{ render_system }
{
     shader_ = shader;
     render_system_->add_draw_child( this );
}


Drawable2D::~Drawable2D()
{
     render_system_->delete_draw_child( this );
}


IRenderSystem2D& Drawable2D::get_render_system() const
{
     return *render_system_;
}

} // namespace _16nar::constructor
