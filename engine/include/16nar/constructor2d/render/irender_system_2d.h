/// @file
/// @brief Header file with IRenderSystem2D interface definition.
#ifndef _16NAR_CONSTRUCTOR_2D_IRENDER_SYSTEM2D_H
#define _16NAR_CONSTRUCTOR_2D_IRENDER_SYSTEM2D_H

#include <16nar/16nardefs.h>
#include <16nar/render/irender_system.h>

namespace _16nar
{

class Camera2D;

} // namespace _16nar


namespace _16nar::constructor2d
{

class Drawable2D;

/// @brief Interface for 2D world state rendering system.
class ENGINE_API IRenderSystem2D : public IRenderSystem
{
public:
     /// @brief Add a drawable object to this system.
     /// @param[in] child pointer to drawable object to be added.
     virtual void add_draw_child( Drawable2D *child ) = 0;

     /// @brief Delete a drawable object from this system, memory will not be freed.
     /// @param[in] child pointer to drawable object to be deleted.
     virtual void delete_draw_child( Drawable2D *child ) = 0;

     /// @brief Handle change of object and adjust for future redrawing.
     /// @param[in] child changed object.
     virtual void handle_change( Drawable2D *child ) = 0;

     /// @brief Set camera of the render system.
     /// @param[in] camera camera of the render system.
     virtual void set_camera( Camera2D *camera ) = 0;

     /// @brief Get camera of the render system.
     /// @return camera of the render system.
     virtual const Camera2D *get_camera() const = 0;
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_IRENDER_SYSTEM2D_H
