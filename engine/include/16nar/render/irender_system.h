/// @file
/// @brief Header file with IRenderSystem interface definition.
#ifndef _16NAR_IRENDER_SYSTEM_H
#define _16NAR_IRENDER_SYSTEM_H

#include <16nar/16nardefs.h>
#include <16nar/render/render_defs.h>

namespace _16nar
{

class Drawable;

/// @brief Interface for world state rendering system.
/// @details Rendering system defines an algorithm for drawing.
/// It is library-agnostic, so algorithm must not care about OpenGL, DirectX, Vulkan etc.
class ENGINE_API IRenderSystem
{
public:
     IRenderSystem()                                               = default;
     IRenderSystem( const IRenderSystem& )                          = delete;
     IRenderSystem( IRenderSystem&& )                               = delete;

     /// @brief Virtual default destructor.
     virtual ~IRenderSystem()                                      = default;

     /// @brief Select and save all objects which will be drawn on the screen.
     virtual void select_objects()                                       = 0;

     /// @brief Draw all objects on the screen.
     virtual void draw_objects()                                         = 0;

     /// @brief Add a drawable object to this system.
     /// @param[in] child pointer to drawable object to be added.
     virtual void add_draw_child( Drawable *child )                      = 0;

     /// @brief Delete a drawable object from this system, memory will not be freed.
     /// @param[in] child pointer to drawable object to be deleted.
     virtual void delete_draw_child( Drawable *child )                   = 0;

     /// @brief Handle change of object and adjust for future redrawing.
     /// @param[in] child changed object.
     virtual void handle_change( Drawable *child )                       = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_IRENDER_SYSTEM_H
