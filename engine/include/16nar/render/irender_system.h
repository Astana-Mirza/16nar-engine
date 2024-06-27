/// @file
/// @brief Header file with IRenderSystem interface definition.
#ifndef _16NAR_IRENDER_SYSTEM_H
#define _16NAR_IRENDER_SYSTEM_H

#include <16nar/16nardefs.h>
#include <16nar/render/render_defs.h>

namespace _16nar
{

class IRenderApi;
class Window;

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

     /// @brief Clear all data and reset to initial state.
     virtual void reset()                                                = 0;

     /// @brief Clear screen.
     virtual void clear_screen()                                         = 0;

     /// @brief Select and save all objects which will be drawn on the screen.
     virtual void select_objects()                                       = 0;

     /// @brief Draw all objects on the screen.
     virtual void draw_objects()                                         = 0;

     /// @brief Get render API of the render system.
     /// @return render API of the render system.
     virtual IRenderApi& get_render_api()                                = 0;

     /// @brief Get window of the render sustem.
     /// @return window of the render sustem.
     virtual Window& get_window()                                        = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_IRENDER_SYSTEM_H
