/// @file Header file with Window class declaration.
#ifndef _16NAR_PLATFORM_SYSTEM_WINDOW_H
#define _16NAR_PLATFORM_SYSTEM_WINDOW_H

#include <16nar/platform/defs.h>

#include <16nar/platform/math/defs.h>

#include <string>

namespace _16nar::system
{

class Monitor;

/// @brief Window of the application.
/// @details Most functions operate with screen coordinates.
/// Screen coordinates are not always mapped to pixels as 1:1.
/// This should be taken in account. All functions must be called
/// from the same thread.
/// 
/// Some Window functions do not work on Wayland due to its limitations.
class NARENGINE_PLATFORM_API Window
{
public:
     /// @brief API for which context will be created.
     enum class ContextApi
     {
          OpenGL,             ///< OpenGL.
          OpenGLES,           ///< OpenGL ES.
          NoApi               ///< no any API (for Vulkan or DirectX).
     };


     /// @brief Settings for opening a window.
     /// @details The settings are global and apply to every window open operation.
     ///
     /// Some (or all) settings may not be applied when opening a window.
     /// It's up to operating system to decide the resulting open settings, these are
     /// just hints, except of @b forward_compatible and @b compat_profile for OpenGL.
     ///
     /// If OpenGL context version 3.3 or higher is stated, core profile will be
     /// requested.
     struct OpenSettings
     {
          /// @brief Graphics API to create context.
          ContextApi api = ContextApi::NoApi;

          /// @brief Bits for red channel. Affects only OpenGL or OpenGL ES. Value -1 means no preference.
          int red_bits = -1;

          /// @brief Bits for green channel. Affects only OpenGL or OpenGL ES. Value -1 means no preference.
          int green_bits = -1;

          /// @brief Bits for blue channel. Affects only OpenGL or OpenGL ES. Value -1 means no preference.
          int blue_bits = -1;

          /// @brief Bits for alpha channel. Affects only OpenGL or OpenGL ES. Value -1 means no preference.
          int alpha_bits = -1;

          /// @brief Bits for stencil testing. Affects only OpenGL or OpenGL ES. Value -1 means no preference.
          int stencil_bits = -1;

          /// @brief Bits for depth testing. Affects only OpenGL or OpenGL ES. Value -1 means no preference.
          int depth_bits = -1;

          /// @brief Count of sampling buffers for MSAA. Affects only OpenGL or OpenGL ES.
          /// @details Typically set to 0, because multisampling is implemented by user.
          int inner_msaa_samples = 0;

          /// @brief Major version of OpenGL context. Affects only OpenGL or OpenGL ES.
          int context_version_major = 3;

          /// @brief Minor version of OpenGL context. Affects only OpenGL or OpenGL ES.
          int context_version_minor = 3;

          /// @brief sRGB support for the window. Affects only OpenGL or OpenGL ES.
          bool inner_srgb_capable = false;

          /// @brief Enable debugging information for OpenGL context. Affects only OpenGL or OpenGL ES.
          bool opengl_debug_context = false;

          /// @brief Should the context be without deprecated functions. Affects only OpenGL >= 3.0.
          bool forward_compatible = true;

          /// @brief Should the context use compatibility profile. Affects only OpenGL.
          /// @details This setting has effect only for OpenGL >= 3.2, for lower versions, OS will choose.
          bool compat_profile = false;

          /// @brief Ability to resize the window.
          bool resizable = true;

          /// @brief Should the window be visible on open.
          /// @details Fullscreen windows are always visible.
          bool visible = true;

          /// @brief Make window with borders and buttons.
          /// @details Undecorated windows cannot be resized by user.
          bool decorated = true;

          /// @brief Should the window be under focus after opening.
          bool focused = true;

          /// @brief Should the window be iconified after losing focus.
          bool auto_iconify = true;

          /// @brief Should the window be always on top.
          bool always_on_top = false;

          /// @brief Should the window be expanded to monitor size.
          bool maximized = false;

          /// @brief Should the cursor be centered after opening. Affects only fulscreen windows.
          bool center_cursor = true;

          /// @brief Should the window have transparent framebuffer.
          bool transparent_framebuffer = false;

          /// @brief Should the window be focused every time it's shown.
          bool focus_on_show = true;

          /// @brief Should the monitor's content scale be taken in account.
          bool scale_to_monitor = false;
     };

     /// @brief Set hints for window opening.
     /// @param[in] settings settings for opening the window.
     static void init_window_hints( const OpenSettings& settings );

     /// @brief Clear hints for window opening.
     static void clear_window_hints();

     /// @brief Move constructor.
     /// @details Opened window should be checked with @b is_open() function.
     /// @param[in] other window to be moved from.
     Window( Window&& other ) noexcept;

     /// @brief Constructor, makes non-fullscreen window.
     /// @details Opened window should be checked with @b is_open() function.
     /// @param[in] size size dimensions of the window, in screen coordinates.
     /// @param[in] title title of the window.
     /// @param[in] settings settings for opening the window.
     Window( math::Vec2i size, std::string_view title );

     /// @brief Constructor, makes fullscreen window on a monitor.
     /// @details Opened window should be checked with @b is_open() function.
     /// @param[in] size size dimensions of the window, in screen coordinates, closest video mode will be requested.
     /// @param[in] title title of the window.
     /// @param[in] monitor monitor on which the window will be displayed.
     Window( math::Vec2i size, std::string_view title, const Monitor& monitor );

     /// @brief Make context of the window to be current for OpenGL rendering.
     void make_context_current();

     /// @brief Switch buffers for OpenGL rendering.
     /// @details Terminates application if there is no current context on the window,
     /// so use this function with caution. Does nothing if window is not opened.
     void swap_buffers();

     /// @brief Destructor, closes the window.
     ~Window();

     /// @brief Close the window.
     /// @details Window cannot be reopened. New window should be created for this purpose.
     /// Operations with closed window will do nothing.
     void close();

     /// @brief Set window position.
     /// @param[in] pos window position, in screen coordinates.
     void set_pos( math::Vec2i pos );

     /// @brief Set title of the window.
     /// @param[in] title new title of the window.
     void set_title( std::string_view title );

     /// @brief Set window's content area size, for fullscreen windows, closest video mode will be requested.
     /// @param[in] size desired size, in screen coordinates.
     void set_size( math::Vec2i size );

     /// @brief Set size limits for window.
     /// @details It is undefined behavior if size limits conflict with aspect ratio.
     /// @param[in] min minimal size of the window.
     /// @param[in] max maximal size of the window.
     void set_size_limit( math::Vec2i min, math::Vec2i max );

     /// @brief Set aspect ratio for window.
     /// @details It is undefined behavior if size limits conflict with aspect ratio.
     /// @param[in] ratio aspect ratio, numerator and denominator.
     void set_aspect_ratio( math::Vec2i ratio );

     /// @brief Switch window to fullscreen mode.
     /// @param[in] monitor monitor to keep fullscreen window.
     void make_fullscreen( const Monitor& monitor );

     /// @brief Switch window to non-fullscreen mode.
     /// @param[in] pos new position of the window, in screen coordinates.
     /// @param[in] size new size of the window, in screen coordinates.
     void make_not_fullscreen( math::Vec2i pos, math::Vec2i size );

     /// @brief Iconify window (to task bar).
     void iconify();

     /// @brief Maximize window, do not confuse with fullscreen.
     void maximize();

     /// @brief Restore window from being iconified or maximized.
     void restore();

     /// @brief Set focus on window.
     void focus();

     /// @brief Request attention, usually blinking on task bar.
     void request_attention();

     /// @brief Set window and decorations opacity.
     /// @details Cannot be used with transparent framebuffer (will do nothing).
     /// @param[in] opacity desired window opacity from 0 to 1.
     void set_opacity( float opacity );

     /// @brief Get underlying handle of the window.
     /// @return underlying handle of the window.
     inline void *get_handle() const noexcept
     {
          return window_;
     }

     /// @brief Check if window is open.
     /// @return true if window is open, false otherwise.
     inline bool is_open() const noexcept
     {
          return window_ != nullptr;
     }

     /// @brief Check if window is focused.
     /// @return true if window is focused, false otherwise.
     bool is_focused() const;

     /// @brief Check if window is iconified (to status bar).
     /// @return true if window is iconified, false otherwise.
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
     /// @return window position.
     math::Vec2i get_pos() const;

     /// @brief Get window's content area size, in screen coordinates.
     /// @return window size.
     math::Vec2i get_size() const;

     /// @brief Get window's edges size, including window's decorations, in screen coordinates.
     /// @param[out] left size of left edge of the window.
     /// @param[out] top size of top edge of the window.
     /// @param[out] right size of right edge of the window.
     /// @param[out] bottom size of bottom edge of the window.
     void get_size_decorated( int& left, int& top, int& right, int& bottom ) const;

     /// @brief Get window's framebuffer size, in pixels.
     /// @return window's framebuffer size.
     math::Vec2i get_framebuffer_size() const;

     /// @brief Get window's content scale.
     /// @return window's content scale.
     math::Vec2f get_content_scale() const;

     /// @brief Get window's monitor, makes sense only for fullscreen windows.
     /// @return window's monitor, default monitor if the window is not fullscreen or an error occured.
     Monitor get_monitor() const;

private:
     Window( const Window& ) = delete;
     Window operator=( const Window& ) = delete;

     void *window_;     ///< inner window handle.
};

} // namespace _16nar::system

#endif // #ifndef _16NAR_PLATFORM_SYSTEM_WINDOW_H
