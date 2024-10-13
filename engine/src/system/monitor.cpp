#include <16nar/system/monitor.h>

#include <16nar/logger/logger.h>

#include <stdexcept>
#include <GLFW/glfw3.h>

namespace _16nar
{

Monitor::ConnectCallback Monitor::connect_callback_ = nullptr;
Monitor::ConnectCallback Monitor::disconnect_callback_ = nullptr;


Monitor::Monitor():
     monitor_{ glfwGetPrimaryMonitor() }
{
     if ( !monitor_ )
     {
          throw std::runtime_error{ "no monitors found" };
     }
     glfwSetMonitorCallback( Monitor::glfw_monitor_callback );
}


Monitor::Monitor( const Monitor& other ):
     monitor_{ other.monitor_ }
{}


Monitor& Monitor::operator=( const Monitor& other )
{
     if ( &other != this )
     {
          monitor_ = other.monitor_;
     }
     return *this;
}


Vec2i Monitor::get_pos() const
{
     Vec2i result;
     glfwGetMonitorPos( monitor_, &result.x(), &result.y() );
     return result;
}


Vec2i Monitor::get_physical_size() const
{
     Vec2i result;
     glfwGetMonitorPhysicalSize( monitor_, &result.x(), &result.y() );
     return result;
}


Vec2f Monitor::get_content_scale() const
{
     Vec2f result;
     glfwGetMonitorContentScale( monitor_, &result.x(), &result.y() );
     return result;
}


IntRect Monitor::get_workarea() const
{
     Vec2i pos;
     int width, height;
     glfwGetMonitorWorkarea( monitor_, &pos.x(), &pos.y(), &width, &height );
     return IntRect{ pos, width, height };
}


std::string Monitor::get_name() const
{
     return std::string{ glfwGetMonitorName( monitor_ ) };
}


Monitor::VideoMode Monitor::get_current_video_mode() const
{
     Monitor::VideoMode vmode;
     const GLFWvidmode *glfw_mode = glfwGetVideoMode( monitor_ );
     vmode.depth.x() = glfw_mode->redBits;
     vmode.depth.y() = glfw_mode->greenBits;
     vmode.depth.z() = glfw_mode->blueBits;
     vmode.size.x() = glfw_mode->width;
     vmode.size.y() = glfw_mode->height;
     vmode.refresh_rate = glfw_mode->refreshRate;
     return vmode;
}


std::vector< Monitor::VideoMode > Monitor::get_video_modes() const
{
     int count;
     const GLFWvidmode *glfw_modes = glfwGetVideoModes( monitor_, &count );
     std::vector< VideoMode > result( count );
     for ( int i = 0; i < count; i++ )
     {
          result[ i ].depth.x() = glfw_modes[ i ].redBits;
          result[ i ].depth.y() = glfw_modes[ i ].greenBits;
          result[ i ].depth.z() = glfw_modes[ i ].blueBits;
          result[ i ].size.x() = glfw_modes[ i ].width;
          result[ i ].size.y() = glfw_modes[ i ].height;
          result[ i ].refresh_rate = glfw_modes[ i ].refreshRate;
     }
     return result;
}


Monitor::GammaRamp Monitor::get_gamma_ramp() const
{
     const GLFWgammaramp *ramp = glfwGetGammaRamp( monitor_ );
     GammaRamp result;
     result.red = ramp->red;
     result.green = ramp->green;
     result.blue = ramp->blue;
     result.size = ramp->size;
     return result;
}


void Monitor::set_gamma( float gamma )
{
     glfwSetGamma( monitor_, gamma );
}


void Monitor::set_gamma_ramp( const Monitor::GammaRamp& ramp )
{
     GLFWgammaramp glfw_ramp;
     glfw_ramp.red = ramp.red;
     glfw_ramp.green = ramp.green;
     glfw_ramp.blue = ramp.blue;
     glfw_ramp.size = static_cast< int >( ramp.size );
     glfwSetGammaRamp( monitor_, &glfw_ramp );
     LOG_16NAR_DEBUG( "Monitor gamma ramp was set (size: " << ramp.size << ")" );
}


std::vector< Monitor > Monitor::get_monitors()
{
     glfwSetMonitorCallback( Monitor::glfw_monitor_callback );

     GLFWmonitor **monitor_array;
     int count;
     monitor_array = glfwGetMonitors( &count );

     if ( monitor_array )
     {
          std::vector< Monitor > result;
          result.reserve( count );
          for ( int i = 0; i < count; i++ )
          {
               result.push_back( monitor_array[ i ] );
          }
          return result;
     }
     return std::vector< Monitor >{};
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


Monitor::Monitor( GLFWmonitor *monitor ) : monitor_{ monitor } {}


void Monitor::glfw_monitor_callback( GLFWmonitor *monitor, int event )
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

} // namespace _16nar
