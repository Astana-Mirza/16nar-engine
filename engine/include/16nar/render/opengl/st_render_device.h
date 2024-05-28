/// @file
/// @brief File with StRenderDevice class definition.
#ifndef _16NAR_OPENGL_ST_RENDER_DEVICE_H
#define _16NAR_OPENGL_ST_RENDER_DEVICE_H

#include <16nar/render/render_defs.h>
#include <16nar/render/opengl/gl_render_defs.h>
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

protected:
     /// @brief Perform render call.
     /// @param[in] params render parameters.
     virtual void do_render( const RenderParams& params );

private:
     const ResourceManagerMap& managers_;    ///< all resource managers.
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_ST_RENDER_DEVICE_H
