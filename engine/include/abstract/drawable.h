/// @file
/// @brief Header file with Slot template class definition.
#ifndef _16NAR_DRAWABLE_H
#define _16NAR_DRAWABLE_H

#include <16nardefs.h>

namespace _16nar
{

/// @brief Abstract base class, providing interface for basic drawing functionality.
class ENGINE_API Drawable
{
public:
     virtual ~Drawable() = default;

     /// @brief Checks if the object is visible.
     bool is_visible() const;

     /// @brief Sets visibility of the object.
     /// @param visible visibility.
     void set_visible( bool visible );

     /// @brief Gets the scene layer of this object.
     int get_layer() const;

     /// @brief Sets scene layer of this object.
     /// @param layer scene layer.
     void set_layer( int layer );

     /// @brief Sets shader of this object.
     /// @param shader pointer to shader.
     void set_shader( Shader *shader );

     /// @brief Gets shader of this object.
     Shader *get_shader() const;

     /// @brief Sets the blend mode of this object.
     /// @param blend blend mode of this object.
     void set_blend( const BlendMode& blend );

     /// @brief Gets the blend mode of this object.
     const BlendMode& get_blend() const;

     /// @brief Sets color of an object.
     /// @param color color of an object
     virtual void set_color( const Color& color ) = 0;

     /// @brief Gets color of an object.
     virtual const Color& get_color() const = 0;

     /// @brief Gets local bounds of an object (in its own coordinates).
     virtual FloatRect get_local_bounds() const = 0;

     /// @brief Draws an object on a given target.
     /// @param target render target where the object should be drawn.
     virtual void draw( RenderTarget& target ) const = 0;

private:
     BlendMode blend_ = BlendAlpha;     ///< mode, defining how pixels of the object will be blended.
     Shader *shader_ = nullptr;         ///< shader to be drawn with the object.
     int layer_ = 0;                    ///< scene layer.
     bool visible_ = true;              ///< visibility.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_H
