/// @file
/// @brief File with IRenderApi interface definition.
#ifndef _16NAR_IRENDER_API_H
#define _16NAR_IRENDER_API_H

#include <16nar/16nardefs.h>
#include <16nar/render/render_defs.h>
#include <16nar/render/irender_device.h>

#include <any>

namespace _16nar
{

/// @brief Interface for low-level graphics API (OpenGL, Vulkan, etc.).
class ENGINE_API IRenderApi
{
public:
     /// @brief Default constructor.
     IRenderApi() = default;

     /// @brief Virtual default destructor.
     virtual ~IRenderApi() = default;

     /// @brief Load a resource with given parameters.
     /// @param[in] type type of the resource.
     /// @param[in] params parameters for loading a resource.
     /// @return identifier of a resource with type.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException, ExceededIdException.
     virtual Resource load( ResourceType type, const std::any& params ) = 0;

     /// @brief Unload a resource.
     /// @param[in] resource identifier of a resource with type.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException.
     virtual void unload( const Resource& resource ) = 0;

     /// @brief Get render device of current graphics API.
     /// @return render device of current graphics API.
     virtual IRenderDevice& get_device() const noexcept = 0;

     /// @brief Process rendering.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException, ExceededIdException.
     virtual void process() = 0;

     /// @brief End current frame and prepare for new frame.
     virtual void end_frame() = 0;

private:
     IRenderApi( const IRenderApi& )            = delete;
     IRenderApi& operator=( const IRenderApi& ) = delete;

protected:
     ResourceManagerMap managers_;                ///< all resource managers.
     std::unique_ptr< IRenderDevice > device_;    ///< render device used to draw graphics.
};

} // namespace _16nar

#endif // #ifndef _16NAR_IRENDER_API_H
