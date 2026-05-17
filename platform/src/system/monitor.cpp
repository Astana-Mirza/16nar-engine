#include <16nar/platform/system/monitor.h>

#include <16nar/platform/logger/logger.h>

#include <stdexcept>

#include <GLFW/glfw3.h>

namespace _16nar::system
{

Monitor::ConnectCallback Monitor::connect_callback_ = nullptr;
Monitor::ConnectCallback Monitor::disconnect_callback_ = nullptr;


Monitor::Monitor():
     monitor_{ ::glfwGetPrimaryMonitor() }
{
     ::glfwSetMonitorCallback( Monitor::glfw_monitor_callback );
}


Monitor::Monitor( const Monitor& other ) noexcept:
     monitor_{ other.monitor_ }
{}


Monitor& Monitor::operator=( const Monitor& other ) noexcept
{
     if ( &other != this )
     {
          monitor_ = other.monitor_;
     }
     return *this;
}


bool Monitor::valid() const noexcept
{
     return monitor_;
}


math::Vec2i Monitor::get_pos() const noexcept
{
     math::Vec2i result;
     ::glfwGetMonitorPos( monitor_, &result.x, &result.y );
     return result;
}


math::Vec2i Monitor::get_physical_size() const noexcept
{
     math::Vec2i result;
     ::glfwGetMonitorPhysicalSize( monitor_, &result.x, &result.y );
     return result;
}


math::Vec2f Monitor::get_content_scale() const noexcept
{
     math::Vec2f result;
     ::glfwGetMonitorContentScale( monitor_, &result.x, &result.y );
     return result;
}


math::IntRect Monitor::get_workarea() const noexcept
{
     math::IntRect area;
     ::glfwGetMonitorWorkarea( monitor_, &area.pos.x, &area.pos.y, &area.size.x, &area.size.y );
     return area;
}


std::string_view Monitor::get_name() const noexcept
{
     const auto *name = ::glfwGetMonitorName( monitor_ );
     return name ? std::string_view{ name } : std::string_view{};
}


Monitor::VideoMode Monitor::get_current_video_mode() const noexcept
{
     Monitor::VideoMode vmode{};
     const auto *glfw_mode = ::glfwGetVideoMode( monitor_ );
     vmode.depth.x = glfw_mode->redBits;
     vmode.depth.y = glfw_mode->greenBits;
     vmode.depth.z = glfw_mode->blueBits;
     vmode.size.x = glfw_mode->width;
     vmode.size.y = glfw_mode->height;
     vmode.refresh_rate = glfw_mode->refreshRate;
     return vmode;
}


std::vector< Monitor::VideoMode > Monitor::get_video_modes() const
{
     int count{};
     const auto *glfw_modes = ::glfwGetVideoModes( monitor_, &count );
     std::vector< VideoMode > result( count );
     for ( int i = 0; i < count; i++ )
     {
          result[ i ].depth.x = glfw_modes[ i ].redBits;
          result[ i ].depth.y = glfw_modes[ i ].greenBits;
          result[ i ].depth.z = glfw_modes[ i ].blueBits;
          result[ i ].size.x = glfw_modes[ i ].width;
          result[ i ].size.y = glfw_modes[ i ].height;
          result[ i ].refresh_rate = glfw_modes[ i ].refreshRate;
     }
     return result;
}


Monitor::GammaRamp Monitor::get_gamma_ramp() const noexcept
{
     const auto *ramp = ::glfwGetGammaRamp( monitor_ );
     GammaRamp result{};
     result.red = ramp->red;
     result.green = ramp->green;
     result.blue = ramp->blue;
     result.size = ramp->size;
     return result;
}


void Monitor::set_gamma( float gamma ) noexcept
{
     ::glfwSetGamma( monitor_, gamma );
}


void Monitor::set_gamma_ramp( const Monitor::GammaRamp& ramp )
{
     ::GLFWgammaramp glfw_ramp{};
     glfw_ramp.red = ramp.red;
     glfw_ramp.green = ramp.green;
     glfw_ramp.blue = ramp.blue;
     glfw_ramp.size = static_cast< int >( ramp.size );
     ::glfwSetGammaRamp( monitor_, &glfw_ramp );
     LOG_16NAR_DEBUG( "Monitor gamma ramp was set (size: %u)", ramp.size );
}


std::vector< Monitor > Monitor::get_monitors()
{
     ::glfwSetMonitorCallback( Monitor::glfw_monitor_callback );

     int count{};
     auto **monitor_array = ::glfwGetMonitors( &count );
     std::vector< Monitor > result( count );

     for ( int i = 0; i < count; i++ )
     {
          result.push_back( monitor_array[ i ] );
     }
     return result;
}


Monitor::ConnectCallback Monitor::set_connect_callback( Monitor::ConnectCallback callback )
{
     ConnectCallback prev = connect_callback_;
     connect_callback_ = callback;
     LOG_16NAR_DEBUG( "Monitor connect callback was set" );
     return prev;
}


Monitor::ConnectCallback Monitor::set_disconnect_callback( Monitor::ConnectCallback callback )
{
     ConnectCallback prev = disconnect_callback_;
     disconnect_callback_ = callback;
     LOG_16NAR_DEBUG( "Monitor disconnect callback was set" );
     return prev;
}


Monitor::Monitor( ::GLFWmonitor *monitor ) noexcept:
     monitor_{ monitor }
{}


void Monitor::glfw_monitor_callback( ::GLFWmonitor *monitor, int event )
{
     if ( event == GLFW_CONNECTED && connect_callback_ )
     {
          LOG_16NAR_INFO( "Monitor was connected" );
          connect_callback_( Monitor{ monitor } );
     }
     else if ( event == GLFW_DISCONNECTED && disconnect_callback_ )
     {
          LOG_16NAR_INFO( "Monitor was disconnected" );
          disconnect_callback_( Monitor{ monitor } );
     }
}

} // namespace _16nar::system
