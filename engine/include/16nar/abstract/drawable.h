/// @file
/// @brief Header file with Slot template class definition.
#ifndef _16NAR_DRAWABLE_H
#define _16NAR_DRAWABLE_H

#include <16nar/16nardefs.h>
#include <16nar/render/quadrant.h>

namespace _16nar
{

class Quadrant;

/// @brief Abstract base class, providing interface for basic drawing functionality.
/// @detail Every drawable object must be in minimal possible quadrant in
/// scene's quadrant tree. The quadrant tree is used for space
/// partitioning to make render faster, because only visible
/// quadrants are checked for drawing.
class ENGINE_API Drawable
{
public:
     /// @brief Constructor which links object to quadrant.
     /// @param quad pointer to quadrant of this object.
     Drawable( Quadrant *quad );

     /// @brief Destructor which removes object from quadrant.
     virtual ~Drawable();

     /// @brief Sets this object to smallest quadrant, in which it fits.
     void fix_quadrant();

     /// @brief Checks if the object is visible.
     virtual bool is_visible() const = 0;

     /// @brief Sets visibility of the object.
     /// @param visible visibility.
     virtual void set_visible( bool visible ) = 0;

     /// @brief Gets the scene layer of this object.
     virtual int get_layer() const = 0;

     /// @brief Sets scene layer of this object.
     /// @param layer scene layer.
     virtual void set_layer( int layer ) = 0;

     /// @brief Sets shader of this object.
     /// @param shader pointer to shader.
     virtual void set_shader( Shader *shader ) = 0;

     /// @brief Gets shader of this object.
     virtual Shader *get_shader() const = 0;

     /// @brief Sets the blend mode of this object.
     /// @param blend blend mode of this object.
     virtual void set_blend( const BlendMode& blend ) = 0;

     /// @brief Gets the blend mode of this object.
     virtual const BlendMode& get_blend() const = 0;

     /// @brief Sets color of an object.
     /// @param color color of an object.
     virtual void set_color( const Color& color ) = 0;

     /// @brief Gets color of an object.
     virtual const Color& get_color() const = 0;

     /// @brief Gets local bounds of an object (in its own coordinates).
     virtual FloatRect get_local_bounds() const = 0;

     /// @brief Draws an object on a given target.
     /// @param target render target where the object should be drawn.
     virtual void draw( RenderTarget& target ) const = 0;

protected:
     /// @brief Check if this object fits in specified quadrant.
     /// @param quad pointer to quadrant to be checked.
     virtual bool check_quadrant( const Quadrant *quad ) const = 0;

     Quadrant *quad_;         ///< pointer to quadrant, in which this object is located.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_H
