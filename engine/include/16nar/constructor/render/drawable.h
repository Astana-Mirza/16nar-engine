/// @file
/// @brief Header file with Drawable abstract class definition.
#ifndef _16NAR_CONSTRUCTOR_DRAWABLE_H
#define _16NAR_CONSTRUCTOR_DRAWABLE_H

#include <16nar/render/render_defs.h>

namespace _16nar::constructor
{

/// @brief Abstract base class providing interface for basic object drawing functionality.
class ENGINE_API Drawable
{
public:
     /// @brief Virtual default destructor.
     virtual ~Drawable() = default;

     /// @brief Get render data of the object.
     /// @return render data of the object.
     virtual DrawInfo get_draw_info() const = 0;

     /// @brief Check if the object is visible.
     /// @return true if the object is visible, false otherwise.
     bool is_visible() const;

     /// @brief Set visibility of the object.
     /// @param[in] visible visibility.
     void set_visible( bool visible );

     /// @brief Get shader of the object.
     /// @return shader of the object.
     const Shader& get_shader() const;

protected:
     Shader shader_;          ///< shader for rendering this object.
     bool visible_ = true;    ///< visibility of the object.
};

} // namespace _16nar::constructor

#endif // #ifndef _16NAR_CONSTRUCTOR_DRAWABLE_H
