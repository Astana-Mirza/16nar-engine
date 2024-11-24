#include <16nar/system/window.h>

#include <16nar/system/monitor.h>
#include <16nar/logger/logger.h>

#include <GLFW/glfw3.h>

#include <utility>
#include <stdexcept>

namespace // anonymous
{

inline void check_window_open( GLFWwindow *window )
{
     if ( !window )
     {
          throw std::runtime_error{ "window is closed" };
     }
}

} // anonymous namespace


namespace _16nar
{

Window::Window( Window&& other ) noexcept:
     window_{ nullptr }
{
     std::swap( other.window_, window_ );
}


Window::Window( Vec2i size, const std::string& title, const OpenSettings& settings ):
     window_{ nullptr }
{
     init_glfw_hints( settings );
     window_ = glfwCreateWindow( size.x(), size.y(), title.c_str(), nullptr, nullptr );
     LOG_16NAR_INFO( "Created " << size.x() << "x" << size.y() << " non-fullscreen window" );
     glfwDefaultWindowHints();
}


Window::Window( Vec2i size, const std::string& title,
     const Monitor& monitor, const OpenSettings& settings ):
     window_{ nullptr }
{
     init_glfw_hints( settings );
     window_ = glfwCreateWindow( size.x(), size.y(), title.c_str(), monitor.monitor_, nullptr );
     LOG_16NAR_INFO( "Created " << size.x() << "x" << size.y() << " fullscreen window" );
     glfwDefaultWindowHints();
}


#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_OPENGL_ES )

Window::Window( Vec2i size, const std::string& title,
     const Window& other, const OpenSettings& settings ):
     window_{ nullptr }
{
     init_glfw_hints( settings );
     window_ = glfwCreateWindow( size.x(), size.y(), title.c_str(), nullptr, other.window_ );
     LOG_16NAR_INFO( "Created " << size.x() << "x" << size.y() << " non-fullscreen window with shared context" );
     glfwDefaultWindowHints();
}


Window::Window( Vec2i size, const std::string& title,
             const Monitor& monitor,
             const Window& other,
             const OpenSettings& settings ):
     window_{ nullptr }
{
     init_glfw_hints( settings );
     window_ = glfwCreateWindow( size.x(), size.y(), title.c_str(), monitor.monitor_, other.window_ );
     LOG_16NAR_INFO( "Created " << size.x() << "x" << size.y() << " fullscreen window with shared context" );
     glfwDefaultWindowHints();
}


void Window::make_context_current()
{
     check_window_open( window_ );
     glfwMakeContextCurrent( window_ );
     LOG_16NAR_DEBUG( "Context made current for window" );
}


void Window::swap_buffers()
{
     if ( !window_ )
     {
          return;
     }
     glfwSwapBuffers( window_ );
}

#endif // NARENGINE_RENDER_OPENGL || NARENGINE_RENDER_OPENGL_ES


Window::~Window()
{
     close();
}


void Window::close()
{
     if ( window_ )
     {
          glfwDestroyWindow( window_ );
          window_ = nullptr;
          LOG_16NAR_INFO( "Closed window" );
     }
}


void Window::set_pos( const Vec2i& pos )
{
     check_window_open( window_ );
     glfwSetWindowPos( window_, pos.x(), pos.y() );
}


void Window::set_title( const std::string& title )
{
     check_window_open( window_ );
     glfwSetWindowTitle( window_, title.c_str() );
}


void Window::set_size( const Vec2i& size )
{
     check_window_open( window_ );
     glfwSetWindowSize( window_, size.x(), size.y() );
}


void Window::set_size_limit( const Vec2i& min, const Vec2i& max )
{
     check_window_open( window_ );
     glfwSetWindowSizeLimits( window_, min.x(), min.y(), max.x(), max.y() );
}


void Window::set_aspect_ratio( int w, int h )
{
     check_window_open( window_ );
     glfwSetWindowAspectRatio( window_, w, h );
}


void Window::make_fullscreen( const Monitor& monitor )
{
     check_window_open( window_ );
     const GLFWvidmode* mode = glfwGetVideoMode( monitor.monitor_ );
     glfwSetWindowMonitor( window_, monitor.monitor_, 0, 0, mode->width, mode->height, mode->refreshRate );
     LOG_16NAR_DEBUG( "Window was made fullscreen" );
}


void Window::make_not_fullscreen( const Vec2i& pos, const Vec2i& size )
{
     check_window_open( window_ );
     glfwSetWindowMonitor( window_, nullptr, pos.x(), pos.y(), size.x(), size.y(), 0 );
     LOG_16NAR_DEBUG( "Window was made non-fullscreen" );
}


void Window::iconify()
{
     check_window_open( window_ );
     glfwIconifyWindow( window_ );
     LOG_16NAR_DEBUG( "Window was iconified" );
}


void Window::maximize()
{
     check_window_open( window_ );
     glfwMaximizeWindow( window_ );
     LOG_16NAR_DEBUG( "Window was maximized" );
}


void Window::restore()
{
     check_window_open( window_ );
     glfwRestoreWindow( window_ );
     LOG_16NAR_DEBUG( "Window was restored" );
}


void Window::focus()
{
     check_window_open( window_ );
     glfwFocusWindow( window_ );
}


void Window::request_attention()
{
     check_window_open( window_ );
     glfwRequestWindowAttention( window_ );
     LOG_16NAR_DEBUG( "Window requested attention" );
}


void Window::set_opacity( float opacity )
{
     check_window_open( window_ );
     if ( glfwGetWindowAttrib( window_, GLFW_TRANSPARENT_FRAMEBUFFER ) )
     {
          throw std::runtime_error{ "setting opacity for transparent framebuffer" };
     }
     glfwSetWindowOpacity( window_, opacity );
}


bool Window::is_open() const noexcept
{
     return window_ != nullptr;
}


bool Window::is_focused() const
{
     check_window_open( window_ );
     return GLFW_TRUE == glfwGetWindowAttrib( window_, GLFW_FOCUSED );
}


bool Window::is_iconified() const
{
     check_window_open( window_ );
     return GLFW_TRUE == glfwGetWindowAttrib( window_, GLFW_ICONIFIED );
}


bool Window::is_maximized() const
{
     check_window_open( window_ );
     return GLFW_TRUE == glfwGetWindowAttrib( window_, GLFW_MAXIMIZED );
}


bool Window::is_resizable() const
{
     check_window_open( window_ );
     return GLFW_TRUE == glfwGetWindowAttrib( window_, GLFW_RESIZABLE );
}


bool Window::is_decorated() const
{
     check_window_open( window_ );
     return GLFW_TRUE == glfwGetWindowAttrib( window_, GLFW_DECORATED );
}


bool Window::is_auto_iconify() const
{
     check_window_open( window_ );
     return GLFW_TRUE == glfwGetWindowAttrib( window_, GLFW_AUTO_ICONIFY );
}


bool Window::is_transparent_framebuffer() const
{
     check_window_open( window_ );
     return GLFW_TRUE == glfwGetWindowAttrib( window_, GLFW_TRANSPARENT_FRAMEBUFFER );
}


bool Window::is_fullscreen() const
{
     check_window_open( window_ );
     return ( glfwGetWindowMonitor( window_ ) != nullptr );
}


Vec2i Window::get_pos() const
{
     check_window_open( window_ );
     Vec2i result;
     glfwGetWindowPos( window_, &result.x(), &result.y() );
     return result;
}


Vec2i Window::get_size() const
{
     check_window_open( window_ );
     Vec2i result;
     glfwGetWindowSize( window_, &result.x(), &result.y() );
     return result;
}


void Window::get_size_decorated( int& left, int& top, int& right, int& bottom ) const
{
     check_window_open( window_ );
     glfwGetWindowFrameSize( window_, &left, &top, &right, &bottom );
}


Vec2i Window::get_framebuffer_size() const
{
     check_window_open( window_ );
     Vec2i result;
     glfwGetFramebufferSize( window_, &result.x(), &result.y() );
     return result;
}


Vec2f Window::get_content_scale() const
{
     check_window_open( window_ );
     Vec2f result;
     glfwGetWindowContentScale( window_, &result.x(), &result.y() );
     return result;
}


Monitor Window::get_monitor() const
{
     check_window_open( window_ );
     GLFWmonitor *mon = glfwGetWindowMonitor( window_ );
     if ( !mon )
     {
          throw std::runtime_error{ "no monitor for non-fullscreen window" };
     }
     return Monitor{ mon };
}


void Window::init_glfw_hints( const OpenSettings& settings ) const
{
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_OPENGL_ES )
     glfwWindowHint( GLFW_RED_BITS, settings.red_bits );
     glfwWindowHint( GLFW_GREEN_BITS, settings.green_bits );
     glfwWindowHint( GLFW_BLUE_BITS, settings.blue_bits );
     glfwWindowHint( GLFW_STENCIL_BITS, settings.stencil_bits );
     glfwWindowHint( GLFW_DEPTH_BITS, settings.depth_bits );
     glfwWindowHint( GLFW_SAMPLES, settings.inner_msaa_samples );
     glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, settings.context_version_major );
     glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, settings.context_version_minor );
     glfwWindowHint( GLFW_SRGB_CAPABLE, settings.inner_srgb_capable ? GLFW_TRUE : GLFW_FALSE );
#endif // NARENGINE_RENDER_OPENGL || NARENGINE_RENDER_OPENGL_ES
     glfwWindowHint( GLFW_TRANSPARENT_FRAMEBUFFER, settings.transparent_framebuffer ? GLFW_TRUE : GLFW_FALSE );
     glfwWindowHint( GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE );
     glfwWindowHint( GLFW_DECORATED, settings.decorated ? GLFW_TRUE : GLFW_FALSE );
     glfwWindowHint( GLFW_FOCUSED, settings.focused ? GLFW_TRUE : GLFW_FALSE );
     glfwWindowHint( GLFW_AUTO_ICONIFY, settings.auto_iconify ? GLFW_TRUE : GLFW_FALSE );
     glfwWindowHint( GLFW_MAXIMIZED, settings.maximized ? GLFW_TRUE : GLFW_FALSE );
     glfwWindowHint( GLFW_CENTER_CURSOR, settings.center_cursor ? GLFW_TRUE : GLFW_FALSE );
     glfwWindowHint( GLFW_SCALE_TO_MONITOR, settings.scale_to_monitor ? GLFW_TRUE : GLFW_FALSE );
     switch( settings.api )
     {
#if defined( NARENGINE_RENDER_OPENGL )
          case ContextApi::OpenGL:
          {
               glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
               if (settings.context_version_major == 3 && settings.context_version_minor >= 0
                || settings.context_version_major > 3 )
               {
                    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, settings.forward_compatible ? GLFW_TRUE : GLFW_FALSE );
               }
               if ( settings.context_version_major == 3 && settings.context_version_minor >= 3
                 || settings.context_version_major > 3 )
               {
                    glfwWindowHint( GLFW_OPENGL_PROFILE, settings.compat_profile ?
                              GLFW_OPENGL_COMPAT_PROFILE : GLFW_OPENGL_CORE_PROFILE );
               }
               else
               {
                    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE );
               }
          }
          break;
#endif
#if defined( NARENGINE_RENDER_OPENGL_ES )
          case ContextApi::OpenGLES:
               glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
          break;
#endif
          default:
               glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
     }
}

} // namespace _16nar
