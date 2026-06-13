/// @file
/// @brief File with MemoryDomain template class definition.
#ifndef _16NAR_PLATFORM_MEMORY_MEMORY_DOMAIN_H
#define _16NAR_PLATFORM_MEMORY_MEMORY_DOMAIN_H

#include <16nar/platform/memory/imemory_domain.h>

namespace _16nar::memory
{

/// @brief Memory domain using template memory resource.
/// @note Memory resource must have @b get_usage() and @b release() member functions.
/// @tparam Resource type of memory resource.
template < typename Resource >
class MemoryDomain : public IMemoryDomain
{
public:
     /// @brief Constructor.
     /// @tparam Args types of memory resource constructor arguments.
     /// @param[in] upstream upstream memory resource.
     /// @param[in] args arguments of memory resource constructor.
     template < typename... Args >
     MemoryDomain( std::pmr::memory_resource *upstream, Args... args ):
          IMemoryDomain::IMemoryDomain( upstream ),
          resource_( upstream, args... )
     {}

     /// @copydoc IMemoryDomain::get_resource()
     std::pmr::memory_resource& get_resource() override
     {
          return resource_;
     }

     /// @copydoc IMemoryDomain::get_usage() const
     MemoryUsage get_usage() const override
     {
          return resource_.get_usage();
     }

protected:
     /// @copydoc IMemoryDomain::reset()
     void reset() override
     {
          resource_.release();
     }

private:
     Resource resource_; ///< memory resource of the domain.
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_MEMORY_DOMAIN_H
