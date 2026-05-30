#include <16nar/platform/system/system.h>

#include <16nar/platform/logger/logger.h>

#include <GLFW/glfw3.h>

namespace _16nar::system
{
namespace
{

bool initialized{ false };

} // anonymous namespace


bool init() noexcept
{
     if ( initialized )
     {
          return true;
     }
     ( void ) ::glfwSetErrorCallback( []( int code, const char *desc ) -> void
     {
          LOG_16NAR_ERROR( "Platform error (%d): %s", ( code, desc ? desc : "unknown" ) );
     } );
     initialized = ( ::glfwInit() == GLFW_TRUE );
     return initialized;
}


void deinit() noexcept
{
     if ( !initialized )
     {
          return;
     }
     ::glfwTerminate();
}


void process_input_events() noexcept
{
     ::glfwPollEvents();
}


void wait_input_events( double timeout ) noexcept
{
     if ( timeout == 0.0 )
     {
          ::glfwWaitEvents();
          return;
     }
     ::glfwWaitEventsTimeout( timeout );
}


void post_empty_event() noexcept
{
     ::glfwPostEmptyEvent();
}


double get_time() noexcept
{
     return ::glfwGetTime();
}


void set_time( double value ) noexcept
{
     ::glfwSetTime( value );
}

} // namespace _16nar::system

