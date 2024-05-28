/// @file
/// @brief File with MtRenderDevice class definition.
#ifndef _16NAR_OPENGL_MT_RENDER_DEVICE_H
#define _16NAR_OPENGL_MT_RENDER_DEVICE_H

#include <16nar/render/opengl/st_render_device.h>

#include <atomic>
#include <array>
#include <queue>

namespace _16nar::opengl
{

/// @brief Class for tracking resources for OpenGL, singlethread profile.
class MtRenderDevice : public StRenderDevice
{
public:
     /// @copydoc StRenderDevice::StRenderDevice(const ResourceManagerMap&)
     MtRenderDevice( const ResourceManagerMap& managers );

     /// @copydoc IRenderDevice::render(const RenderParams&)
     virtual void render( const RenderParams& params ) override;

     /// @copydoc IRenderDevice::process_render_queue()
     virtual void process_render_queue() override;

     /// @copydoc IRenderDevice::end_frame()
     virtual void end_frame() override;

private:
     std::array< std::queue< RenderParams >, _16nar_saved_frames > render_queue_;    ///< queue of render requests.
     std::atomic_size_t frame_index_;                                                ///< index of the frame being rendered.
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_MT_RENDER_DEVICE_H
