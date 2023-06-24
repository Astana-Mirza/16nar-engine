/// @file
/// @brief Header file with RenderSystem interface definition.
#ifndef _16NAR_RENDER_SYSTEM_H
#define _16NAR_RENDER_SYSTEM_H

#include <16nar/16nardefs.h>

namespace _16nar
{

class Drawable;
class RenderDevice;

/// @brief Interface for world state rendering system.
/// @detail Rendering system defines an algorithm for drawing.
/// It is library-agnostic, so algorithm must not care about OpenGL, DirectX, Vulkan etc.
class ENGINE_API RenderSystem
{
public:
     RenderSystem()                      = default;
     RenderSystem( const RenderSystem& ) = delete;
     RenderSystem( RenderSystem&& )      = delete;

     virtual ~RenderSystem() = default;

     /// @brief Start rendering a frame.
     /// @param[in] view view used to define rendering area.
     /// @param[in] device device used for rendering.
     virtual void start_render( const View& view, RenderDevice& device ) = 0;

     /// @brief Finish rendering a frame.
     /// @param[in] device device used for rendering.
     virtual void finish_render( RenderDevice& device ) = 0;

     /// @brief Add a drawable object to this system.
     /// @param[in] child pointer to drawable object to be added.
     virtual void add_draw_child( Drawable *child ) = 0;

     /// @brief Delete a drawable object from this system, memory will not be freed.
     /// @param[in] child pointer to drawable object to be deleted.
     virtual void delete_draw_child( Drawable *child ) = 0;

     /// @brief Handle change of object and adjust for future redrawing.
     /// @param[in] child changed object.
     virtual void handle_change( Drawable *child ) = 0;
};

} // namespace _16nar

#endif  // #ifndef _16NAR_RENDER_SYSTEM_H
