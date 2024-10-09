/// @file Header file with Window class declaration.
#ifndef _16NAR_WINDOW_H
#define _16NAR_WINDOW_H

#include <16nar/16nardefs.h>
#include <16nar/math/vec.h>

#include <string>

struct GLFWwindow;

namespace _16nar
{

/// @brief API for which context will be created.
enum class ContextApi
{
#if defined( NARENGINE_RENDER_OPENGL )
     OpenGL,             ///< OpenGL.
#endif
#if defined( NARENGINE_RENDER_OPENGL_ES )
     OpenGLES,           ///< OpenGL ES.
#endif
     NoApi               ///< no any API (for Vulkan or DirectX).
};


/// @brief Settings for opening a window.
/// @details Some (or all) settings may not be applied when opening a window.
/// It's up to operating system to decide the resulting open settings, these are
/// just hints, except of forward_compatible and compat_profile for OpenGL.
///
/// If OpenGL context version 3.3 or higher is stated, core profile will be
/// requested.
struct OpenSettings
{
     ContextApi api               = ContextApi::OpenGL; ///< API to create context.
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_OPENGL_ES )
     int  red_bits                = 8;       ///< bits for red channel (OpenGL or OpenGL ES).
     int  green_bits              = 8;       ///< bits for green channel (OpenGL or OpenGL ES).
     int  blue_bits               = 8;       ///< bits for blue channel (OpenGL or OpenGL ES).
     int  stencil_bits            = 8;       ///< bits for stencil testing (OpenGL or OpenGL ES).
     int  depth_bits              = 24;      ///< bits for depth testing (OpenGL or OpenGL ES).
     int  inner_msaa_samples      = 0;       ///< count of sampling buffers for MSAA (OpenGL or OpenGL ES).
     int  context_version_major   = 3;       ///< major version of OpenGL context (OpenGL or OpenGL ES).
     int  context_version_minor   = 3;       ///< minor version of OpenGL context (OpenGL or OpenGL ES).
     bool inner_srgb_capable      = false;   ///< sRGB support for the window (OpenGL or OpenGL ES).
#endif // NARENGINE_RENDER_OPENGL || NARENGINE_RENDER_OPENGL_ES
#if defined( NARENGINE_RENDER_OPENGL )
     bool forward_compatible      = true;    ///< should the context be without deprecated functions (OpenGL >= 3.0 only).
     bool compat_profile          = false;   ///< should the context use compatibility profile
                                             /// (OpenGL >= 3.2 only, for lower versions, OS will choose).
#endif // NARENGINE_RENDER_OPENGL
     bool transparent_framebuffer = false;   ///< ability to make transparent framebuffer.
     bool resizable               = true;    ///< ability to resize the window.
     bool decorated               = true;    ///< make window with borders and buttons.
                                             ///  Undecorated windows cannot be resized bu user.
     bool focused                 = true;    ///< should the window be under focus after opening.
     bool auto_iconify            = true;    ///< should the window be iconified after losing focus.
     bool maximized               = false;   ///< should the window be expanded to monitor size.
     bool center_cursor           = false;   ///< should the cursor be centered after opening (fulscreen only).
     bool scale_to_monitor        = false;   ///< should the monitor's content scale be taken in account.
};


class Monitor;

/// @brief Window of the application.
/// @details Most functions operate with screen coordinates.
/// Screen coordinates are not always mapped to pixels as 1:1.
/// This should be taken in account. All functions must be called from main thread.
/// 
/// Some Window functions do not work on Wayland due to its limitations.
/// For example, it is impossible to know window position on the screen.
class ENGINE_API Window
{
public:
     Window( const Window& ) = delete;

     /// @brief Move constructor.
     /// @param[in] other window to be moved from.
     Window( Window&& other ) noexcept;

     /// @brief Constructor, makes non-fullscreen window.
     /// @param[in] size size dimensions of the window, in screen coordinates.
     /// @param[in] title title of the window.
     /// @param[in] settings settings for opening the window.
     Window( Vec2i size, const std::string& title, const OpenSettings& settings = OpenSettings{} );

     /// @brief Constructor, makes fullscreen window on a monitor.
     /// @param[in] size size dimensions of the window, in screen coordinates, closest video mode will be requested.
     /// @param[in] title title of the window.
     /// @param[in] monitor monitor on which the window will be displayed.
     Window( Vec2i size, const std::string& title, const Monitor& monitor, const OpenSettings& settings = OpenSettings{} );

#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_OPENGL_ES )
     /// @brief Constructor, makes non-fullscreen window sharing context with another window.
     /// @param[in] size size dimensions of the window, in screen coordinates.
     /// @param[in] title title of the window.
     /// @param[in] other window to share the context with. On Windows, its context must not be active.
     /// @param[in] settings settings for opening the window.
     Window( Vec2i size, const std::string& title, const Window& other, const OpenSettings& settings = OpenSettings{} );

     /// @brief Constructor, makes fullscreen window on a monitor.
     /// @param[in] size size dimensions of the window, in screen coordinates, closest video mode will be requested.
     /// @param[in] title title of the window.
     /// @param[in] other window to share the context with. On Windows, its context must not be active.
     /// @param[in] monitor monitor on which the window will be displayed.
     Window( Vec2i size, const std::string& title,
             const Monitor& monitor,
             const Window& other,
             const OpenSettings& settings = OpenSettings{} );

     /// @brief Make context of the window to be current for OpenGL rendering.
     void make_context_current();

     /// @brief Switch buffers for OpenGL rendering.
     /// @details Throws std::runtime_error if there is no current context on the window.
     void swap_buffers();

#endif // NARENGINE_RENDER_OPENGL || NARENGINE_RENDER_OPENGL_ES

     /// @brief Destructor, closes the window.
     ~Window();

     /// @brief Close the window.
     /// @details Window cannot be reopened. New window should be created for this purpose.
     /// Operations with closed window will cause runtime_error.
     void close();

     /// @brief Set window position.
     /// @param[in] pos window position, in screen coordinates.
     /// @details Always throws std::runtime_error on Wayland.
     void set_pos( const Vec2i& pos );

     /// @brief Set title of the window.
     /// @param[in] title new title of the window.
     void set_title( const std::string& title );

     /// @brief Set window's content area size, for fullscreen windows, closest video mode will be requested.
     /// @param[in] size desired size, in screen coordinates.
     /// @details Always throws std::runtime_error on Wayland.
     void set_size( const Vec2i& size );

     /// @brief Set size limits for window.
     /// @details It is undefined behavior if size limits conflict with aspect ratio.
     /// Always throws std::runtime_error on Wayland.
     /// @param[in] min minimal size of the window.
     /// @param[in] max maximal size of the window.
     void set_size_limit( const Vec2i& min, const Vec2i& max );

     /// @brief Set aspect ratio for window.
     /// @details It is undefined behavior if size limits conflict with aspect ratio.
     /// Always throws std::runtime_error on Wayland.
     /// @param[in] w width (numerator).
     /// @param[in] h height (denominator).
     void set_aspect_ratio( int w, int h );

     /// @brief Switch window to fullscreen mode.
     /// @param[in] monitor monitor to keep fullscreen window.
     void make_fullscreen( const Monitor& monitor );

     /// @brief Switch window to non-fullscreen mode.
     /// @param[in] pos new position of the window, in screen coordinates.
     /// @param[in] size new size of the window, in screen coordinates.
     void make_not_fullscreen( const Vec2i& pos, const Vec2i& size );

     /// @brief Iconify window (to task bar).
     void iconify();

     /// @brief Maximize window, do not confuse with fullscreen.
     void maximize();

     /// @brief Restore window from being iconified or maximized.
     void restore();

     /// @brief Set focus on window.
     /// @details Always throws std::runtime_error on Wayland.
     void focus();

     /// @brief Request attention, usually blinking on task bar.
     void request_attention();

     /// @brief Set window and decorations opacity.
     /// @details Cannot be used with transparent framebuffer (will throw runtime_error).
     /// @param[in] opacity desired window opacity from 0 to 1.
     void set_opacity( float opacity );

     /// @brief Check if window is open.
     /// @return true if window is open, false otherwise.
     bool is_open() const;

     /// @brief Check if window is focused.
     /// @return true if window is focused, false otherwise.
     bool is_focused() const;

     /// @brief Check if window is iconified (to status bar).
     /// @details Always returns false on Wayland.
     /// @return true if window is iconified, false otherwise, always false on Wayland.
     bool is_iconified() const;

     /// @brief Check if window is maximized.
     /// @return true if window is maximized, false otherwise.
     bool is_maximized() const;

     /// @brief Check if window is resizable.
     /// @return true if window is resizable, false otherwise.
     bool is_resizable() const;

     /// @brief Check if window is decorated.
     /// @return true if window is decorated, false otherwise.
     bool is_decorated() const;

     /// @brief Check if window is auto iconified on focus loss.
     /// @return true if window is auto iconified, false otherwise.
     bool is_auto_iconify() const;

     /// @brief Check if window has transparent framebuffer.
     /// @return true if window has transparent framebuffer, false otherwise.
     bool is_transparent_framebuffer() const;

     /// @brief Check if the window is in fullscreen mode.
     /// @return true if window is in fullscreen mode, false otherwise.
     bool is_fullscreen() const;

     /// @brief Get window position, in screen coordinates.
     /// @details Always throws std::runtime_error on Wayland.
     /// @return window position.
     Vec2i get_pos() const;

     /// @brief Get window's content area size, in screen coordinates.
     /// @return window size.
     Vec2i get_size() const;

     /// @brief Get window's edges size, including window's decorations, in screen coordinates.
     /// @param[out] left size of left edge of the window.
     /// @param[out] top size of top edge of the window.
     /// @param[out] right size of right edge of the window.
     /// @param[out] bottom size of bottom edge of the window.
     void get_size_decorated( int& left, int& top, int& right, int& bottom ) const;

     /// @brief Get window's framebuffer size, in pixels.
     /// @return window's framebuffer size.
     Vec2i get_framebuffer_size() const;

     /// @brief Get window's content scale.
     /// @return window's content scale.
     Vec2f get_content_scale() const;

     /// @brief Get window's monitor.
     /// @details For fullscreen windows only. Throws runtime_error for non-fullscreen windows.
     /// @return window's monitor.
     Monitor get_monitor() const;

private:
     /// @brief Set GLFW hints for window opening.
     /// @param[in] settings settings for opening the window.
     void init_glfw_hints( const OpenSettings& settings ) const;

     GLFWwindow *window_;     ///< inner window handle.
};

} // namespace _16nar

#endif // #ifndef _16NAR_WINDOW_H
