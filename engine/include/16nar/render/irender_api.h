/// @file
/// @brief File with IRenderApi interface definition.
#ifndef _16NAR_OPENGL_IRENDER_API_H
#define _16NAR_OPENGL_IRENDER_API_H

#include <16nar/16nardefs.h>
#include <16nar/render/render_defs.h>

#include <any>

namespace _16nar
{

class IRenderDevice;


/// @brief Interface for low-level graphics API (OpenGL, Vulkan, etc.).
class ENGINE_API IRenderApi
{
public:
     /// @brief Virtual default destructor.
     virtual ~IRenderApi() = default;
     IRenderApi()          = default;

     /// @brief Load a resource with given parameters.
     /// @param[in] type type of the resource.
     /// @param[in] params parameters for loading a resource.
     /// @return identifier of a resource with type.
     virtual Resource load( ResourceType type, const std::any& params ) = 0;

     /// @brief Unload a resource.
     /// @param[in] resource identifier of a resource with type.
     virtual void unload( const Resource& resource ) = 0;

     /// @brief Get render device of current graphics API.
     /// @return render device of current graphics API.
     virtual IRenderDevice& get_device() const = 0;

     /// @brief Process rendering.
     virtual void process() = 0;

     /// @brief End current frame and prepare for new frame.
     virtual void end_frame() = 0;

protected:
     IRenderApi( const IRenderApi& )            = delete;
     IRenderApi& operator=( const IRenderApi& ) = delete;
};

} // namespace _16nar

#endif // #ifndef _16NAR_OPENGL_IRENDER_API_H
