/// @file
/// @brief Header file with Slot template class definition.
#ifndef _16NAR_DRAWABLE_H
#define _16NAR_DRAWABLE_H

#include <cassert>

#include <16nar/16nardefs.h>
#include <16nar/abstract/render_system.h>

namespace _16nar
{

/// @brief Abstract base class providing interface for basic drawing functionality.
class ENGINE_API Drawable
{
public:
     /// @brief Constructor which links object to render system.
     /// @param[in] render_system render system used to draw this object.
     Drawable( RenderSystem *render_system ) : render_system_{ render_system }
     {
          assert( render_system );
          render_system_->add_draw_child( this );
     }

     /// @brief Destructor which removes object from render system.
     virtual inline ~Drawable() { render_system_->delete_draw_child( this ); }

     inline RenderSystem& get_render_system() { return *render_system_; }

     /// @brief Check if the object is visible.
     virtual bool is_visible() const = 0;

     /// @brief Set visibility of the object.
     /// @param[in] visible visibility.
     virtual void set_visible( bool visible ) = 0;

     /// @brief Get the scene layer of this object.
     virtual int get_layer() const = 0;

     /// @brief Set scene layer of this object.
     /// @param[in] layer scene layer.
     virtual void set_layer( int layer ) = 0;

     /// @brief Set shader of this object.
     /// @param[in] shader pointer to shader.
     virtual void set_shader( Shader *shader ) = 0;

     /// @brief Get shader of this object.
     virtual Shader *get_shader() const = 0;

     /// @brief Set the blend mode of this object.
     /// @param[in] blend blend mode of this object.
     virtual void set_blend( const BlendMode& blend ) = 0;

     /// @brief Get the blend mode of this object.
     virtual const BlendMode& get_blend() const = 0;

     /// @brief Set color of an object.
     /// @param[in] color color of an object.
     virtual void set_color( const Color& color ) = 0;

     /// @brief Get color of an object.
     virtual const Color& get_color() const = 0;

     /// @brief Get local bounds of an object (in its own coordinates).
     virtual FloatRect get_local_bounds() const = 0;

     /// @brief Get global bounds of an object (in world coordinates).
     virtual FloatRect get_global_bounds() const = 0;

     /// @brief Get render data of the object.
     virtual RenderData get_render_data() const = 0;

protected:
     RenderSystem *render_system_;         ///< render system which draws this object.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_H
