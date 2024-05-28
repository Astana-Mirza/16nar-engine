/// @file
/// @brief File with IRenderDevice interface definition.
#ifndef _16NAR_IRENDER_DEVICE_H
#define _16NAR_IRENDER_DEVICE_H

#include <16nar/16nardefs.h>
#include <16nar/render/render_defs.h>

namespace _16nar
{

/// @brief Inteface for rendering graphic primitives on the screen.
class ENGINE_API IRenderDevice
{
public:
     /// @brief Virtual default destructor.
     virtual ~IRenderDevice() = default;

     /// @brief Schedules a draw call with given parameters.
     /// @details Order of draw calls is preserved even if they are not immediate.
     /// @param[in] params render parameters.
     virtual void render( const RenderParams& params ) = 0;

     /// @brief Process all requests in render queue.
     virtual void process_render_queue() {}

     /// @brief Swap queues and prepare for new frame.
     virtual void end_frame() {}
};

} // namespace _16nar

#endif // #ifndef _16NAR_IRENDER_DEVICE_H
