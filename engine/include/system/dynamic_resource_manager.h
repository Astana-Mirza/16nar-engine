/// @file
/// @brief File with DynamicResourceManager class definition.
#ifndef _16NAR_DYNAMIC_RESOURCE_MANAGER_H
#define _16NAR_DYNAMIC_RESOURCE_MANAGER_H

#include <16nardefs.h>

#include <string>
#include <unordered_map>
#include <stdexcept>

namespace _16nar
{

/// @brief Singleton for handling dynamically loaded resources.
/// @tparam T type of loaded resource.
template < typename T >
class DynamicResourceManager
{
public:
     /// @brief Get the only one instance of the class.
     static DynamicResourceManager& instance()
     {
          static DynamicResourceManager obj;
          return obj;
     }

     // no copy construction and assignment.
     DynamicResourceManager( const DynamicResourceManager& ) = delete;
     void operator=( const DynamicResourceManager& )         = delete;

     /// @brief Load (reload) a resource from storage.
     /// @param name relative path to a resource, aka its name.
     void load( const std::string& name )
     {
#ifdef USE_SFML
          if ( !resources_[ name ].loadFromFile( name ) )
          {
               throw std::runtime_error{ "Cannot load resource from file " + name };
          }
#endif // #ifdef USE_SFML
     }


     /// @brief Get resource with given name, load if needed.
     /// @param name relative path to a resource, aka its name.
     const T& get( const std::string& name )
     {
          auto it = resources_.find( name );
          if ( it == resources_.end() )
          {
               load( name );
               return resources_.at( name );
          }
          return it->second;
     }


     /// @brief Free resource memory.
     /// @param name relative path to a resource, aka its name.
     void free( const std::string& name )
     {
          resources_.erase( name );
     }

private:
     DynamicResourceManager() = default;

     std::unordered_map< std::string, T > resources_;       ///< set of named resources.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DYNAMIC_RESOURCE_MANAGER_H
