/// @file
/// @brief Header file with RenderSystem interface definition.
#ifndef _16NAR_RENDER_SYSTEM_H
#define _16NAR_RENDER_SYSTEM_H

namespace _16nar
{

class Drawable;

/// @brief Interface for world rendering system.
/// @detail Rendering system defines an algorithm for drawing.
/// It is library-agnostic, so algorithm must not care about OpenGL, DirectX, Vulkan etc.
class RenderSystem
{
public:
     virtual ~RenderSystem() = default;

     /// @brief Start rendering a frame.
     virtual void start_render() = 0;

     /// @brief Finish rendering a frame.
     virtual void finish_render() = 0;

     /// @brief Add a drawable object to this quadrant.
     /// @param[in] child pointer to drawable object to be added.
     virtual void add_draw_child( Drawable* child ) = 0;

     /// @brief Delete a drawable object from this quadrant, memory will not be freed.
     /// @param[in] child pointer to drawable object to be deleted.
     virtual void delete_draw_child( Drawable* child ) = 0;
};

} // namespace _16nar

#endif  // #ifndef _16NAR_RENDER_SYSTEM_H
