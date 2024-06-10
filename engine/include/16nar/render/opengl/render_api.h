/// @file
/// @brief File with RenderApi class definition.
#ifndef _16NAR_OPENGL_RENDER_API_H
#define _16NAR_OPENGL_RENDER_API_H

#include <16nar/render/irender_api.h>

#include <16nar/render/iresource_manager.h>
#include <16nar/render/irender_device.h>
#include <16nar/render/render_defs.h>

#include <unordered_map>
#include <memory>

namespace _16nar::opengl
{

/// @brief Class for rendering primitives with OpenGL.
class ENGINE_API RenderApi : public IRenderApi
{
public:
     /// @brief Constructor.
     /// @param[in] profile type of profile used in application.
     RenderApi( ProfileType profile );

     /// @copydoc IRenderApi::load(ResourceType, const std::any&)
     virtual Resource load( ResourceType type, const std::any& params ) override;

     /// @copydoc IRenderApi::unload(const Resource&)
     virtual void unload( const Resource& resource ) override;

     /// @copydoc IRenderApi::get_device()
     virtual IRenderDevice& get_device() const override;

     /// @copydoc IRenderApi::process()
     virtual void process() override;

     /// @copydoc IRenderApi::end_frame()
     virtual void end_frame() override;

private:
     ResourceManagerMap managers_;                ///< all resource managers.
     std::unique_ptr< IRenderDevice > device_;    ///< render device used to draw graphics.
};

} // namespace _16nar::opengl

#endif // _16NAR_OPENGL_RENDER_API_H
