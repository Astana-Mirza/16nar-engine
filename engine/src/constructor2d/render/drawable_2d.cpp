#include <16nar/constructor2d/render/drawable_2d.h>

#include <16nar/constructor2d/render/irender_system_2d.h>

#include <cassert>

namespace _16nar::constructor2d
{

Drawable2D::Drawable2D( const Shader& shader ) noexcept:
     render_system_{ nullptr }, layer_{ 0 }
{
     shader_ = shader;
}


Drawable2D::~Drawable2D()
{
     if ( render_system_ )
     {
          render_system_->delete_draw_child( this );
     }
}


void Drawable2D::set_render_system( IRenderSystem2D *render_system )
{
     if ( render_system == render_system_ )
     {
          // already set
          return;
     }
     if ( render_system_ )
     {
          render_system_->delete_draw_child( this );
     }
     if ( render_system )
     {
          render_system->add_draw_child( this );
     }
     render_system_ = render_system;
}


IRenderSystem2D *Drawable2D::get_render_system() const noexcept
{
     return render_system_;
}


int Drawable2D::get_layer() const noexcept
{
     return layer_;
}


void Drawable2D::set_layer( int layer ) noexcept
{
     layer_ = layer;
}

} // namespace _16nar::constructor2d
