/// @file
/// @brief Header file with Drawable abstract class definition.
#ifndef _16NAR_DRAWABLE_H
#define _16NAR_DRAWABLE_H

#include <16nar/16nardefs.h>
#include <cassert>

namespace _16nar
{

class RenderSystem;

/// @brief Abstract base class providing interface for basic drawing functionality.
class ENGINE_API Drawable
{
public:
     /// @brief Constructor which links object to render system.
     /// @param[in] render_system render system used to draw this object.
     Drawable( RenderSystem *render_system ):
          render_system_{ render_system }, visible_{ true }
     {
          assert( render_system );
          render_system_->add_draw_child( this );
     }

     /// @brief Destructor which removes object from render system.
     virtual ~Drawable() { render_system_->delete_draw_child( this ); }

     /// @brief Get render system used for drawing the object.
     RenderSystem& get_render_system() { return *render_system_; }

     /// @brief Get render data of the object.
     virtual RenderData get_render_data() const = 0;

     /// @brief Check if the object is visible.
     bool is_visible() const { return visible_; }

     /// @brief Set visibility of the object.
     /// @param[in] visible visibility.
     void set_visible( bool visible ) { visible_ = visible; }

     /// @brief Set shader of this object.
     /// @param[in] shader pointer to shader.
     void set_shader( Shader shader ) { shader_ = shader; }

     /// @brief Get shader of this object.
     Shader get_shader() const { return shader_; }

protected:
     Shader shader_;                       ///< shader for rendering this object.
     RenderSystem *render_system_;         ///< render system which draws this object.
     bool visible_;                        ///< visibility of the object.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_H
