/// @file Header file with Monitor class declaration.
#ifndef _16NAR_MONITOR_H
#define _16NAR_MONITOR_H

#include <16nar/16nardefs.h>
#include <16nar/math/vec.h>
#include <16nar/math/rectangle.h>

#include <vector>
#include <string>

struct GLFWmonitor;

namespace _16nar
{

/// @brief Class for monitor representation.
/// @detail User can change settings of monitor and make use of several monitors.
/// For example, when creating a fullscreen window, the user needs to specify
/// which monitor it will occupy.
/// 
/// Monitors operate with virtual coordinates, which are not the same as pixels.
/// 
/// All functions must be called from main thread.
/// Monitor object becomes invalid after connection or disconnection of any monitor.
class ENGINE_API Monitor
{
public:
     friend class Window;

     /// @brief Callback called when monitor is connected or disconnected.
     using ConnectCallback = void ( * )( Monitor );

     /// @brief Video mode of a monitor.
     struct VideoMode
     {
          Vec3i depth;        ///< bit depth for red, green and blue channels.
          Vec2i size;         ///< size of the video mode, in screen coordinates.
          int refresh_rate;   ///< refresh rate of the video mode, in Hz.
     };

     /// @brief Gamma ramp for a monitor.
     /// @detail Work with gamma is restricted in Wayland.
     struct GammaRamp
     {
          uint16_t *red;      ///< array of values for red channel.
          uint16_t *green;    ///< array of values for green channel.
          uint16_t *blue;     ///< array of values for blue channel.
          size_t size;        ///< number of elements in each array (must be 256 on Linux).
     };

     /// @brief Constructor with primary monitor.
     /// @detail Throws runtime_error if no monitors were found.
     Monitor();

     /// @brief Copy constructor.
     /// @param[in] other monitor to be copied.
     Monitor( const Monitor& other );

     /// @brief Copy assignment operator.
     /// @param[in] other monitor to be copied.
     /// @return current object which was assigned to.
     Monitor& operator=( const Monitor& other );

     /// @brief Get virtual position of the monitor's viewport.
     /// @return virtual position of the monitor's viewport.
     Vec2i get_pos() const;

     /// @brief Get monitor's size, in millimeters.
     /// @return monitor's size, in millimeters.
     Vec2i get_physical_size() const;

     /// @brief Get content scale ratio for X and Y.
     /// @return content scale ratio.
     Vec2f get_content_scale() const;

     /// @brief Get virtual area without OS taskbars.
     /// @return virtual area without OS taskbars.
     IntRect get_workarea() const;

     /// @brief Get monitor name.
     /// @return monitor name.
     std::string get_name() const;

     /// @brief Get current video mode of the monitor.
     /// @return current video mode of the monitor.
     VideoMode get_current_video_mode() const;

     /// @brief Get supported video modes.
     /// @return supported video modes.
     std::vector< VideoMode > get_video_modes() const;

     /// @brief Get gamma ramp applied to the monitor.
     /// @return gamma ramp applied to the monitor.
     GammaRamp get_gamma_ramp() const;

     /// @brief Set additional software gamma for the monitor.
     /// @detail New gamma ramp is calculated and applied.
     /// Setting gamma to 1.0 will produce default monitor behavior.
     /// @param[in] gamma gamma value (exponent).
     void set_gamma( float gamma );

     /// @brief Set gamma ramp for the monitor.
     /// @param[in] ramp gamma ramp.
     void set_gamma_ramp( const GammaRamp& ramp );

     /// @brief Get all connected monitors.
     /// @return all connected monitors.
     static std::vector< Monitor > get_monitors();

     /// @brief Set callback for monitor connection.
     /// @param[in] callback callback for monitor connection.
     /// @return previous callback (nullptr if none).
     static ConnectCallback set_connect_callback( ConnectCallback callback );

     /// @brief Set callback for monitor disconnection.
     /// @param[in] callback callback for monitor disconnection.
     /// @return previous callback (nullptr if none).
     static ConnectCallback set_disconnect_callback( ConnectCallback callback );

private:
     /// @brief Constructor for inner use only.
     /// @param[in] monitor inner representation of monitor.
     Monitor( GLFWmonitor *monitor );

     /// @brief Inner callback function for GLFW monitor.
     /// @param[in] monitor monitor handle.
     /// @param[in] event received event number.
     static void glfw_monitor_callback( GLFWmonitor *monitor, int event );

     GLFWmonitor *monitor_;                       ///< inner representation of monitor.
     static ConnectCallback connect_callback_;    ///< callback for monitor connection.
     static ConnectCallback disconnect_callback_; ///< callback for monitor disconnection.
};

} // namespace _16nar

#endif // #ifndef _16NAR_MONITOR_H
