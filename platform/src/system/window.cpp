#include <16nar/platform/system/window.h>

#include <16nar/platform/system/monitor.h>
#include <16nar/platform/logger/logger.h>

#include <GLFW/glfw3.h>

#define NARENGINE_CHECK_WINDOW( X ) \
{ \
     if ( !( X ) ) \
     { \
          LOG_16NAR_ERROR( "Window is not opened" ); \
          return; \
     } \
}


#define NARENGINE_CHECK_WINDOW_VAL( X, Y ) \
{ \
     if ( !( X ) ) \
     { \
          LOG_16NAR_ERROR( "Window is not opened" ); \
          return ( Y ); \
     } \
}


namespace _16nar::system
{

void Window::init_window_hints( const OpenSettings& settings )
{
     auto check_int = []( int val ){ return ( val < 0 ) ? GLFW_DONT_CARE : val; };
     auto check_bool = []( bool val ){ return val ? GLFW_TRUE : GLFW_FALSE; };

     if ( settings.api != ContextApi::NoApi )
     {
          ::glfwWindowHint( GLFW_RED_BITS, check_int( settings.red_bits ) );
          ::glfwWindowHint( GLFW_GREEN_BITS, check_int( settings.green_bits ) );
          ::glfwWindowHint( GLFW_BLUE_BITS, check_int( settings.blue_bits ) );
          ::glfwWindowHint( GLFW_ALPHA_BITS, check_int( settings.alpha_bits ) );
          ::glfwWindowHint( GLFW_STENCIL_BITS, check_int( settings.stencil_bits ) );
          ::glfwWindowHint( GLFW_DEPTH_BITS, check_int( settings.depth_bits ) );
          ::glfwWindowHint( GLFW_SAMPLES, check_int( settings.inner_msaa_samples ) );
          ::glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, settings.context_version_major );
          ::glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, settings.context_version_minor );
          ::glfwWindowHint( GLFW_SRGB_CAPABLE, check_bool( settings.inner_srgb_capable ) );
          ::glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, check_bool( settings.opengl_debug_context ) );
     }

     ::glfwWindowHint( GLFW_RESIZABLE, check_bool( settings.resizable ) );
     ::glfwWindowHint( GLFW_VISIBLE, check_bool( settings.visible ) );
     ::glfwWindowHint( GLFW_DECORATED, check_bool( settings.decorated ) );
     ::glfwWindowHint( GLFW_FOCUSED, check_bool( settings.focused ) );
     ::glfwWindowHint( GLFW_AUTO_ICONIFY, check_bool( settings.auto_iconify ) );
     ::glfwWindowHint( GLFW_FLOATING, check_bool( settings.always_on_top ) );
     ::glfwWindowHint( GLFW_MAXIMIZED, check_bool( settings.maximized ) );
     ::glfwWindowHint( GLFW_CENTER_CURSOR, check_bool( settings.center_cursor ) );
     ::glfwWindowHint( GLFW_TRANSPARENT_FRAMEBUFFER, check_bool( settings.transparent_framebuffer ) );
     ::glfwWindowHint( GLFW_FOCUS_ON_SHOW, check_bool( settings.focus_on_show ) );
     ::glfwWindowHint( GLFW_SCALE_TO_MONITOR, check_bool( settings.scale_to_monitor ) );

     switch ( settings.api )
     {
          case ContextApi::OpenGL:
          {
               ::glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
               if ( ( settings.context_version_major == 3
                    && settings.context_version_minor >= 0 )
                    || settings.context_version_major > 3 )
               {
                    ::glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT,
                         check_bool( settings.forward_compatible ) );
               }
               if ( ( settings.context_version_major == 3
                    && settings.context_version_minor >= 3 )
                    || settings.context_version_major > 3 )
               {
                    ::glfwWindowHint( GLFW_OPENGL_PROFILE, settings.compat_profile ?
                              GLFW_OPENGL_COMPAT_PROFILE : GLFW_OPENGL_CORE_PROFILE );
               }
               else
               {
                    ::glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE );
               }
               break;
          }
          case ContextApi::OpenGLES:
               ::glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
               break;
          default:
               ::glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
     }
}


void Window::clear_window_hints()
{
     ::glfwDefaultWindowHints();
}


Window::Window( Window&& other ) noexcept:
     window_{ nullptr }
{
     std::swap( other.window_, window_ );
}


Window::Window( math::Vec2i size, std::string_view title ):
     window_{ nullptr }
{
     window_ = ::glfwCreateWindow( size.x, size.y, title.data(), nullptr, nullptr );
}


Window::Window( math::Vec2i size, std::string_view title, const Monitor& monitor ):
     window_{ nullptr }
{
     window_ = ::glfwCreateWindow( size.x, size.y, title.data(),
          static_cast< ::GLFWmonitor * >( monitor.monitor_ ), nullptr );
}


void Window::make_context_current()
{
     NARENGINE_CHECK_WINDOW( window_ );
     LOG_16NAR_DEBUG( "Making context current for the window" );
     ::glfwMakeContextCurrent( static_cast< ::GLFWwindow * >( window_ ) );
}


void Window::swap_buffers()
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwSwapBuffers( static_cast< ::GLFWwindow * >( window_ ) );
}


Window::~Window()
{
     close();
}


void Window::close()
{
     if ( window_ )
     {
          ::glfwDestroyWindow( static_cast< ::GLFWwindow * >( window_ ) );
          window_ = nullptr;
          LOG_16NAR_INFO( "Closed window" );
     }
}


void Window::set_pos( math::Vec2i pos )
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwSetWindowPos( static_cast< ::GLFWwindow * >( window_ ), pos.x, pos.y );
}


void Window::set_title( std::string_view title )
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwSetWindowTitle( static_cast< ::GLFWwindow * >( window_ ), title.data() );
}


void Window::set_size( math::Vec2i size )
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwSetWindowSize( static_cast< ::GLFWwindow * >( window_ ), size.x, size.y );
}


void Window::set_size_limit( math::Vec2i min, math::Vec2i max )
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwSetWindowSizeLimits( static_cast< ::GLFWwindow * >( window_ ), min.x, min.y, max.x, max.y );
}


void Window::set_aspect_ratio( math::Vec2i ratio )
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwSetWindowAspectRatio( static_cast< ::GLFWwindow * >( window_ ), ratio.x, ratio.y );
}


void Window::make_fullscreen( const Monitor& monitor )
{
     NARENGINE_CHECK_WINDOW( window_ );
     const auto *mode = ::glfwGetVideoMode( static_cast< ::GLFWmonitor * >( monitor.monitor_ ) );
     ::glfwSetWindowMonitor( static_cast< ::GLFWwindow * >( window_ ),
          static_cast< ::GLFWmonitor * >( monitor.monitor_ ),
          0, 0, mode->width, mode->height, mode->refreshRate );
     LOG_16NAR_DEBUG( "Window was made fullscreen" );
}


void Window::make_not_fullscreen( math::Vec2i pos, math::Vec2i size )
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwSetWindowMonitor( static_cast< ::GLFWwindow * >( window_ ),
          nullptr, pos.x, pos.y, size.x, size.y, 0 );
     LOG_16NAR_DEBUG( "Window was made non-fullscreen" );
}


void Window::iconify()
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwIconifyWindow( static_cast< ::GLFWwindow * >( window_ ) );
     LOG_16NAR_DEBUG( "Window was iconified" );
}


void Window::maximize()
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwMaximizeWindow( static_cast< ::GLFWwindow * >( window_ ) );
     LOG_16NAR_DEBUG( "Window was maximized" );
}


void Window::restore()
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwRestoreWindow( static_cast< ::GLFWwindow * >( window_ ) );
     LOG_16NAR_DEBUG( "Window was restored" );
}


void Window::focus()
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwFocusWindow( static_cast< ::GLFWwindow * >( window_ ) );
}


void Window::request_attention()
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwRequestWindowAttention( static_cast< ::GLFWwindow * >( window_ ) );
     LOG_16NAR_DEBUG( "Window requested attention" );
}


void Window::set_opacity( float opacity )
{
     NARENGINE_CHECK_WINDOW( window_ );
     if ( ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ), GLFW_TRANSPARENT_FRAMEBUFFER ) )
     {
          LOG_16NAR_ERROR( "Cannot set opacity for transparent framebuffer" );
          return;
     }
     ::glfwSetWindowOpacity( static_cast< ::GLFWwindow * >( window_ ), opacity );
}


bool Window::is_focused() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return GLFW_TRUE == ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ), GLFW_FOCUSED );
}


bool Window::is_iconified() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return GLFW_TRUE == ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ), GLFW_ICONIFIED );
}


bool Window::is_maximized() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return GLFW_TRUE == ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ), GLFW_MAXIMIZED );
}


bool Window::is_resizable() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return GLFW_TRUE == ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ), GLFW_RESIZABLE );
}


bool Window::is_decorated() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return GLFW_TRUE == ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ), GLFW_DECORATED );
}


bool Window::is_auto_iconify() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return GLFW_TRUE == ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ), GLFW_AUTO_ICONIFY );
}


bool Window::is_transparent_framebuffer() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return GLFW_TRUE == ::glfwGetWindowAttrib( static_cast< ::GLFWwindow * >( window_ ),
          GLFW_TRANSPARENT_FRAMEBUFFER );
}


bool Window::is_fullscreen() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, false );
     return ( ::glfwGetWindowMonitor( static_cast< ::GLFWwindow * >( window_ ) ) != nullptr );
}


math::Vec2i Window::get_pos() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, math::Vec2i{} );
     math::Vec2i result;
     ::glfwGetWindowPos( static_cast< ::GLFWwindow * >( window_ ), &result.x, &result.y );
     return result;
}


math::Vec2i Window::get_size() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, math::Vec2i{} );
     math::Vec2i result;
     ::glfwGetWindowSize( static_cast< ::GLFWwindow * >( window_ ), &result.x, &result.y );
     return result;
}


void Window::get_size_decorated( int& left, int& top, int& right, int& bottom ) const
{
     NARENGINE_CHECK_WINDOW( window_ );
     ::glfwGetWindowFrameSize( static_cast< ::GLFWwindow * >( window_ ), &left, &top, &right, &bottom );
}


math::Vec2i Window::get_framebuffer_size() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, math::Vec2i{} );
     math::Vec2i result;
     ::glfwGetFramebufferSize( static_cast< ::GLFWwindow * >( window_ ), &result.x, &result.y );
     return result;
}


math::Vec2f Window::get_content_scale() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, math::Vec2f{} );
     math::Vec2f result;
     ::glfwGetWindowContentScale( static_cast< ::GLFWwindow * >( window_ ), &result.x, &result.y );
     return result;
}


Monitor Window::get_monitor() const
{
     NARENGINE_CHECK_WINDOW_VAL( window_, Monitor{} );
     auto *mon = ::glfwGetWindowMonitor( static_cast< ::GLFWwindow * >( window_ ) );
     if ( !mon )
     {
          return Monitor{};
     }
     return Monitor{ mon };
}

} // namespace _16nar::system
