/// @file
/// @brief File with ServiceLocator class definition.
#ifndef _16NAR_CORE_LOADER_SERVICE_LOCATOR_H
#define _16NAR_CORE_LOADER_SERVICE_LOCATOR_H

#include <16nar/core/defs.h>

#include <memory_resource>
#include <unordered_map>
#include <typeindex>

namespace _16nar::loader
{

/// @brief Storage of various interfaces.
class NARENGINE_CORE_API ServiceLocator
{
public:
     /// @brief Constructor.
     /// @param[in] resource memory resource for utility data allocations.
     ServiceLocator( std::pmr::memory_resource& resource ): services_( resource ) {}

     /// @brief Add service to the locator.
     /// @tparam T type of service.
     /// @param[in] service service to be added.
     template < typename T >
     void add( T *service )
     {
          services_[ std::type_index{ typeid( T ) } ] = service;
     }

     /// @brief Get service with given type.
     /// @tparam T type of service.
     /// @return requested services if it exists, nullptr otherwise.
     template < typename T >
     T *get() const noexcept
     {
          const auto iter = services_.find( std::type_index{ typeid( T ) } );
          return ( services_.cend() == iter ) ? nullptr : static_cast< T * >( iter->second );
     }

private:
     std::pmr::unordered_map< std::type_index, void * > services_;    ///< registered services.
};

} // namespace _16nar::loader

#endif // #ifndef _16NAR_CORE_LOADER_SERVICE_LOCATOR_H
