/// @file
/// @brief File with MtRenderDevice class definition.
#ifndef _16NAR_OPENGL_MT_RENDER_DEVICE_H
#define _16NAR_OPENGL_MT_RENDER_DEVICE_H

#include <16nar/render/opengl/st_render_device.h>

#include <atomic>
#include <array>
#include <queue>
#include <functional>

namespace _16nar::opengl
{

/// @brief Class for tracking resources for OpenGL, multithread profile.
/// @details Order of all calls is preserved even if they are not immediate.
class MtRenderDevice : public StRenderDevice
{
public:
     /// @brief Constructor.
     /// @param[in] managers resource managers used in rendering.
     MtRenderDevice( const ResourceManagerMap& managers );

     /// @copydoc IRenderDevice::render(const RenderParams&)
     virtual void render( const RenderParams& params ) override;

     /// @copydoc IRenderDevice::set_viewport(const IntRect&)
     virtual void set_viewport( const IntRect& rect ) override;

     /// @copydoc IRenderDevice::set_depth_test_state(bool)
     virtual void set_depth_test_state( bool enable ) override;

     /// @copydoc IRenderDevice::bind_shader(const Shader&)
     virtual void bind_shader( const Shader& shader ) override;

     /// @copydoc IRenderDevice::set_shader_params(const ShaderSetupFunction&)
     virtual void set_shader_params( const ShaderSetupFunction& setup ) override;

     /// @copydoc IRenderDevice::bind_framebuffer(const FrameBuffer&)
     virtual void bind_framebuffer( const FrameBuffer& framebuffer ) override;

     /// @copydoc IRenderDevice::clear(bool, bool, bool)
     virtual void clear( bool color, bool depth, bool stencil ) override;

     /// @copydoc IRenderDevice::process_render_queue()
     virtual void process_render_queue() override;

     /// @copydoc IRenderDevice::end_frame()
     virtual void end_frame() override;

private:
     /// @brief Render function which can be stored and called later.
     using Request = std::function< void() >;

     std::array< std::queue< Request >, _16nar_saved_frames > render_queue_;    ///< queue of render requests.
     std::atomic_size_t frame_index_;                                           ///< index of the frame being rendered.
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_MT_RENDER_DEVICE_H
