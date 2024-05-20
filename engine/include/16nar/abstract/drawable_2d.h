/// @file
/// @brief Header file with Drawable2D abstract class definition.
#ifndef _16NAR_DRAWABLE_2D_H
#define _16NAR_DRAWABLE_2D_H

#include <16nar/abstract/drawable.h>
#include <16nar/math/rectangle.h>

namespace _16nar
{

/// @brief Abstract base class providing interface for basic drawing functionality.
class ENGINE_API Drawable2D : public Drawable
{
public:
     /// @brief Get the scene layer of this object.
     virtual int get_layer() const               = 0;

     /// @brief Set scene layer of this object.
     /// @param[in] layer scene layer.
     virtual void set_layer( int layer )         = 0;

     /// @brief Get local bounds of an object (in its own coordinates).
     virtual FloatRect get_local_bounds() const  = 0;

     /// @brief Get global bounds of an object (in world coordinates).
     virtual FloatRect get_global_bounds() const = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_2D_H
