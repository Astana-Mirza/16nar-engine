/// @file
/// @brief Header file with RenderSystem interface definition.
#ifndef _16NAR_RENDER_SYSTEM_H
#define _16NAR_RENDER_SYSTEM_H

#include <16nar/16nardefs.h>
#include <16nar/render/render_defs.h>

namespace _16nar
{

class Camera;
class Drawable;

/// @brief Interface for world state rendering system.
/// @details Rendering system defines an algorithm for drawing.
/// It is library-agnostic, so algorithm must not care about OpenGL, DirectX, Vulkan etc.
class ENGINE_API RenderSystem
{
public:
     RenderSystem()                                                = default;
     RenderSystem( const RenderSystem& )                            = delete;
     RenderSystem( RenderSystem&& )                                 = delete;

     virtual ~RenderSystem()                                       = default;

     /// @brief Start rendering a frame.
     /// @param[in] camera camera used to define rendering area.
     virtual void start_render( const Camera& camera )                   = 0;

     /// @brief Finish rendering a frame.
     virtual void finish_render()                                        = 0;

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

#endif  // #ifndef _16NAR_RENDER_SYSTEM_H
