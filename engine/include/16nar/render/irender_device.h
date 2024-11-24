/// @file
/// @brief File with IRenderDevice interface definition.
#ifndef _16NAR_IRENDER_DEVICE_H
#define _16NAR_IRENDER_DEVICE_H

#include <16nar/16nardefs.h>
#include <16nar/render/render_defs.h>

#include <16nar/math/rectangle.h>

namespace _16nar
{

class IShaderProgram;

/// @brief Inteface for rendering graphic primitives on the screen.
class ENGINE_API IRenderDevice
{
public:
     /// @brief Default constructor.
     IRenderDevice() = default;

     /// @brief Virtual default destructor.
     virtual ~IRenderDevice() = default;

     /// @brief Schedules a draw call with given parameters.
     /// @param[in] params render parameters.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException.
     virtual void render( const RenderParams& params ) = 0;

     /// @brief Set viewport for rendering.
     /// @param[in] rect rectangle of a viewport.
     /// @throws May throw implementation-defined exceptions.
     virtual void set_viewport( const IntRect& rect ) = 0;

     /// @brief Set state of depth testing.
     /// @param[in] enable should the depth testing be enabled.
     /// @throws May throw implementation-defined exceptions.
     virtual void set_depth_test_state( bool enable ) = 0;

     /// @brief Bind shader program.
     /// @param[in] shader target shader resource identifier.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException.
     virtual void bind_shader( const Shader& shader ) = 0;

     /// @brief Set currently bound shader program parameters (uniforms).
     /// @details setup function may be called in other thread and in other frame,
     /// so it must not capture context by reference.
     /// @param[in] setup function for setting uniforms.
     /// @throws May throw implementation-defined exceptions.
     virtual void set_shader_params( const ShaderSetupFunction& setup ) = 0;

     /// @brief Bind framebuffer for rendering.
     /// @details Framebuffer ID 0 can be specified to bind default framebuffer.
     /// @param[in] framebuffer target framebuffer resource identifier.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException.
     virtual void bind_framebuffer( const FrameBuffer& framebuffer ) = 0;

     /// @brief Clear currently bound framebuffer.
     /// @param[in] color should the color buffer be cleared.
     /// @param[in] depth should the depth buffer be cleared.
     /// @param[in] stencil should the stencil buffer be cleared.
     /// @throws May throw implementation-defined exceptions.
     virtual void clear( bool color, bool depth, bool stencil ) = 0;

     /// @brief Process all requests in render queue.
     /// @throws May throw implementation-defined exceptions.
     /// Current implementations may throw ResourceException.
     virtual void process_render_queue() {}

     /// @brief Swap queues and prepare for new frame.
     virtual void end_frame() {}

private:
     IRenderDevice( const IRenderDevice& )            = delete;
     IRenderDevice& operator=( const IRenderDevice& ) = delete;
};

} // namespace _16nar

#endif // #ifndef _16NAR_IRENDER_DEVICE_H
