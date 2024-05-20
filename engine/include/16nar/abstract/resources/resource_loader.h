/// @file
/// @brief Header file with ResourceLoader template class definitions.
#ifndef _16NAR_RESOURCE_LOADER_H
#define _16NAR_RESOURCE_LOADER_H

#include <16nar/16nardefs.h>

namespace _16nar
{

/// @brief Resource handler for any resource loader.
/// @tparam T type of resource loader.
template < typename T > struct Handler {};


/// @brief Base class for all resource loaders.
template < typename T >
class ResourceLoader
{
public:
     /// @brief Load resource using its information.
     /// @return true on success, false otherwise.
     bool load() const
     {
          return static_cast< const T* >( this )->load_impl();
     }

     /// @brief Get resource handler.
     /// @return resource handler, default initialized if the resource is not yet loaded.
     Handler< T > get_handler() const
     {
          return static_cast< const T* >( this )->get_handler_impl();
     }
};

} // namespace _16nar

#endif // #ifndef _16NAR_RESOURCE_LOADER_H
