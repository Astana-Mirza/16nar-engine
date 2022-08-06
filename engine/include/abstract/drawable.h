/// @file
/// Header file with Slot template class definition.
#ifndef _16NAR_DRAWABLE_H
#define _16NAR_DRAWABLE_H

#include <16nardefs.h>

namespace _16nar
{

/// Abstract base class, providing interface for basic drawing functionality.
class ENGINE_API Drawable : public DrawableBase
{
public:
     virtual ~Drawable() = default;

     /// Checks if the object is visible.
     bool is_visible() const;

     /// Sets visibility of the object.
     /// @param visible visibility.
     void set_visible( bool visible );

     /// Gets the scene layer of this object.
     int get_layer() const;

     /// Sets scene layer of this object.
     /// @param layer scene layer.
     void set_layer( int layer );

     /// Sets color of an object.
     /// @param color color of an object
     virtual void set_color( const Color& color ) = 0;

     /// Gets color of an object.
     virtual const Color& get_color() const = 0;

     /// Gets local bounds of an object (in its own coordinates).
     virtual FloatRect get_local_bounds() const = 0;

private:
     int layer_ = 0;               ///< scene layer.
     bool visible_ = true;         ///< visibility.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_H
