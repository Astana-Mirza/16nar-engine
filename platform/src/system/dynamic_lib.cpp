#include <16nar/platform/system/dynamic_lib.h>

#include <16nar/platform/logger/logger.h>

#include <stdexcept>

#ifdef __linux__
#    include <dlfcn.h>
#elif _WIN32
#    if !defined( WIN32_LEAN_AND_MEAN )
#         define WIN32_LEAN_AND_MEAN
#    endif
#    include <windows.h>
#endif

namespace _16nar::system
{

DynamicLib::DynamicLib( std::string_view name ):
     handle_{}
{
#ifdef __linux__
     handle_ = ::dlopen( name.data(), RTLD_LAZY );
     if ( !handle_ )
     {
          LOG_16NAR_ERROR( "Cannot open library %s: %s", name.data(), ::dlerror() );
          return;
     }
     ::dlerror();	// clear errors
#elif _WIN32
     handle_ = static_cast< void * >( ::LoadLibrary( name.data() ) );
     if ( !handle_ )
     {
          LOG_16NAR_ERROR( "Cannot open library %s: %s", name.data(), ::GetLastError() );
          return;
     }
#endif
     LOG_16NAR_INFO( "Successfully loaded dynamic library '%s'", name.data() );
}


DynamicLib::DynamicLib( DynamicLib&& lib ) noexcept
{
     std::swap( handle_, lib.handle_ );
}


DynamicLib& DynamicLib::operator=( DynamicLib&& lib ) noexcept
{
     if ( this != &lib )
     {
          handle_ = lib.handle_;
          lib.handle_ = nullptr;
     }
     return *this;
}


DynamicLib::~DynamicLib() noexcept
{
     if ( is_loaded() )
     {
#ifdef __linux__
          ::dlclose( handle_ );
#elif _WIN32
          ::FreeLibrary( static_cast< HMODULE >( handle_ ) );
#endif
     }
}


bool DynamicLib::is_loaded() const noexcept
{
     return handle_;
}


void *DynamicLib::get_symbol( std::string_view name ) const
{
     if ( !is_loaded() )
     {
          return nullptr;
     }
#ifdef __linux__
     void *sym = ::dlsym( handle_, name.data() );
     const char *error = ::dlerror();
     if ( error )
     {
          LOG_16NAR_ERROR( "Cannot load symbol '%s': %s", name.data(), error );
          return nullptr;
     }
     return sym;
#elif _WIN32
     void *sym = ::GetProcAddress( static_cast< HMODULE >( handle_ ), name.data() );
     if ( !sym )
     {
          LOG_16NAR_ERROR( "Cannot load symbol '%s': %s", name.data(), ::GetLastError() );
     }
     return sym;
#endif
     return nullptr;
}

} // namespace _16nar::system
