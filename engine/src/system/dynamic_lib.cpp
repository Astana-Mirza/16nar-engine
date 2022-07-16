#include <system/dynamic_lib.h>

#include <stdexcept>
#include <dlfcn.h>

namespace _16nar
{

DynamicLib::DynamicLib( const std::string& name ):
     name_{ name }
{
     handle_ = dlopen( name_.c_str(), RTLD_LAZY );
     if ( !handle_ )
     {
          throw std::runtime_error{ "Cannot open library: " + std::string{ dlerror() } };
     }
     dlerror();	// clear errors
}


DynamicLib::DynamicLib( DynamicLib&& lib )
{
     std::swap( name_, lib.name_ );
     std::swap( handle_, lib.handle_ );
}


DynamicLib& DynamicLib::operator= ( DynamicLib&& lib )
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
          dlclose( handle_ );
     }
}


void *DynamicLib::get_symbol( const std::string& name ) const
{
     void *sym = dlsym( handle_, name.c_str() );
     const char *error = dlerror();
     if ( error )
     {
          throw std::runtime_error{ "Cannot load symbol " + name +
                                    " from library " + name_ + ": " + error };
     }
     return sym;
}

} // namespace _16nar
