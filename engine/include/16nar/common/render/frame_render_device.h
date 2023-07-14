/// @file
/// @brief Header file with FrameRenderDevice class definition.
#ifndef _16NAR_FRAME_RENDER_DEVICE_H
#define _16NAR_FRAME_RENDER_DEVICE_H

#include <16nar/abstract/render_device.h>

#include <queue>
#include <vector>

namespace _16nar
{

/// @brief Render device which can save frame data in queue.
class ENGINE_API FrameRenderDevice : public RenderDevice
{
public:
     using FrameQueue = std::queue< std::vector< RenderDevice::DeviceData > >;

     /// @brief Constructor.
     FrameRenderDevice( const std::shared_ptr< RenderWindow >& window );

     /// @brief Add data for rendering.
     /// @detail Data will be rendered in order of adding,
     /// it may be significant if drawing with multiple layers.
     /// @param[in] data render data.
     void add_data( const DeviceData& data ) override;

     /// @brief End one render frame, not neccessarily render it (for render, use flush).
     void end_frame() override;

     /// @brief Flush all saved render data for one frame, which means that data will be rendered.
     void flush() override;

private:
     FrameQueue frames_;                          ///< all frames data.
     std::shared_ptr< RenderWindow > window_;     ///< pointer to main window of the game.
};

} // namespace _16nar

#endif // #ifndef _16NAR_FRAME_RENDER_DEVICE_H
