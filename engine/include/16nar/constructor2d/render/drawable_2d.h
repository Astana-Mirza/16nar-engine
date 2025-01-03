/// @file
/// @brief Header file with Drawable2D abstract class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_DRAWABLE_2D_H
#define _16NAR_CONSTRUCTOR_2D_DRAWABLE_2D_H

#include <16nar/render/drawable.h>
#include <16nar/math/rectangle.h>

namespace _16nar::constructor2d
{

class IRenderSystem2D;

/// @brief Abstract base class providing interface for basic drawing functionality.
class ENGINE_API Drawable2D : public Drawable
{
public:
     /// @brief Constructor.
     /// @param[in] shader shader used to draw this object.
     explicit Drawable2D( const Shader& shader ) noexcept;

     /// @brief Destructor.
     /// @details Destructor removes object from render system, which means that
     /// current render system pointer must be valid and point to some render system (or be nullptr).
     virtual ~Drawable2D();

     /// @brief Set render system which will draw the object.
     /// @details This member function removes this object from old render system
     /// (if any) and adds it to new @b render_system. It means that both render systems
     /// pointers, new and previous, must be valid and point to some render system (or be nullptr).
     /// @param[in] render_system render system for drawing the object.
     void set_render_system( IRenderSystem2D *render_system );

     /// @brief Get render system used for drawing the object.
     /// @return render system used for drawing the object (may be nullptr if not set).
     IRenderSystem2D *get_render_system() const noexcept;

     /// @brief Get the scene layer of the object.
     /// @return scene layer of the object.
     int get_layer() const noexcept;

     /// @brief Set scene layer of this object.
     /// @param[in] layer scene layer.
     void set_layer( int layer ) noexcept;

     /// @brief Get local bounds of the object (in its own coordinates).
     /// @return local bounds of the object.
     virtual FloatRect get_local_bounds() const  = 0;

     /// @brief Get global bounds of the object (in world coordinates).
     /// @return global bounds of the object.
     virtual FloatRect get_global_bounds() const = 0;

private:
     IRenderSystem2D *render_system_;   ///< render system which draws this object.
     int layer_;                        ///< layer of this object which affects drawing order.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_DRAWABLE_2D_H
