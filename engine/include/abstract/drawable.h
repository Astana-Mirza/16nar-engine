/// @file
/// Header file with Slot template class definition.
#ifndef _16NAR_DRAWABLE_H
#define _16NAR_DRAWABLE_H

#include <16nardefs.h>

namespace _16nar
{

/// Abstract base class, providing interface for basic drawing functionality.
class Drawable : public DrawableBase
{
public:
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

     virtual ~Drawable() = default;

private:
     int layer_ = 0;               ///< scene layer.
     bool visible_ = true;         ///< visibility.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_H
