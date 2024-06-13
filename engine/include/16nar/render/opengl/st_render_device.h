/// @file
/// @brief File with StRenderDevice class definition.
#ifndef _16NAR_OPENGL_ST_RENDER_DEVICE_H
#define _16NAR_OPENGL_ST_RENDER_DEVICE_H

#include <16nar/render/render_defs.h>
#include <16nar/render/opengl/utils.h>
#include <16nar/render/irender_device.h>

namespace _16nar::opengl
{
 
/// @brief Class for tracking resources for OpenGL, singlethread profile.
class StRenderDevice : public IRenderDevice
{
public:
     /// @brief Constructor.
     /// @param[in] managers resource managers used in rendering.
     StRenderDevice( const ResourceManagerMap& managers );

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

private:
     const ResourceManagerMap& managers_;              ///< all resource managers.
     Handler< ResourceType::Shader > current_shader_;  ///< currently bound shader program handler.
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_ST_RENDER_DEVICE_H
