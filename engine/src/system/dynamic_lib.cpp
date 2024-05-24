#include <16nar/system/dynamic_lib.h>

#include <16nar/logger/logger.h>

#include <stdexcept>

#ifdef __linux__
#    include <dlfcn.h>
#elif _WIN32
#    include <windows.h>
#endif

namespace _16nar
{

DynamicLib::DynamicLib( const std::string& name ):
     name_{ name }
{
#ifdef __linux__
     handle_ = dlopen( name_.c_str(), RTLD_LAZY );
     if ( !handle_ )
     {
          throw std::runtime_error{ "Cannot open library: " + std::string{ dlerror() } };
     }
     dlerror();	// clear errors
#elif _WIN32
     handle_ = static_cast< void * >( LoadLibrary( name_.c_str() ) );
     if ( !handle_ )
     {
         throw std::runtime_error{ "Cannot open library: error " + std::to_string( GetLastError() ) };
     }
#endif
     LOG_16NAR_INFO( "Successfully loaded dynamic library " << name );
}


DynamicLib::DynamicLib( DynamicLib&& lib ) noexcept
{
     std::swap( name_, lib.name_ );
     std::swap( handle_, lib.handle_ );
}


DynamicLib& DynamicLib::operator= ( DynamicLib&& lib ) noexcept
{
     if ( this != &lib )
     {
          handle_ = nullptr;
          std::swap( name_, lib.name_ );
          std::swap( handle_, lib.handle_ );
     }
     return *this;
}


DynamicLib::~DynamicLib()
{
     if ( handle_ )
     {
#ifdef __linux__
          dlclose( handle_ );
#elif _WIN32
          FreeLibrary( static_cast< HMODULE >( handle_ ) );
#endif
          LOG_16NAR_DEBUG( "Successfully unloaded dynamic library" );
     }
}


void *DynamicLib::get_symbol( const std::string& name ) const
{
#ifdef __linux__
     void *sym = dlsym( handle_, name.c_str() );
     const char *error = dlerror();
     if ( error )
     {
          throw std::runtime_error{ "Cannot load symbol " + name +
                                    " from library " + name_ + ": " + error };
     }
#elif _WIN32
     void* sym = GetProcAddress( static_cast< HMODULE >( handle_ ), name.c_str() );
     if ( !sym )
     {
          throw std::runtime_error{ "Cannot load symbol " + name + " from library " +
                                    name_ + ": error " + std::to_string( GetLastError() ) };
     }
#endif
     return sym;
}

} // namespace _16nar
