/// @file
/// @brief Header file with Drawable2D abstract class definition.
#ifndef _16NAR_CONSTRUCTOR_DRAWABLE_2D_H
#define _16NAR_CONSTRUCTOR_DRAWABLE_2D_H

#include <16nar/constructor/render/drawable.h>
#include <16nar/math/rectangle.h>

namespace _16nar::constructor
{

class IRenderSystem2D;

/// @brief Abstract base class providing interface for basic drawing functionality.
class ENGINE_API Drawable2D : public Drawable
{
public:
     /// @brief Constructor which links object to render system.
     /// @param[in] shader shader used to draw this object.
     /// @param[in] render_system render system used to draw this object.
     Drawable2D( const Shader& shader, IRenderSystem2D *render_system );

     /// @brief Destructor which removes object from render system.
     virtual ~Drawable2D();

     /// @brief Get render system used for drawing the object.
     /// @return render system used for drawing the object.
     IRenderSystem2D& get_render_system() const;

     /// @brief Get the scene layer of the object.
     /// @return scene layer of the object.
     virtual int get_layer() const               = 0;

     /// @brief Set scene layer of this object.
     /// @param[in] layer scene layer.
     virtual void set_layer( int layer )         = 0;

     /// @brief Get local bounds of the object (in its own coordinates).
     /// @return local bounds of the object.
     virtual FloatRect get_local_bounds() const  = 0;

     /// @brief Get global bounds of the object (in world coordinates).
     /// @return global bounds of the object.
     virtual FloatRect get_global_bounds() const = 0;

protected:
     IRenderSystem2D* render_system_;   ///< render system which draws this object.
};

} // namespace _16nar::constructor

#endif // #ifndef _16NAR_CONSTRUCTOR_DRAWABLE_2D_H
