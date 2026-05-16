/// @file
/// @brief File with MemoryManager class definition.
#ifndef _16NAR_PLATFORM_MEMORY_MEMORY_MANAGER_H
#define _16NAR_PLATFORM_MEMORY_MEMORY_MANAGER_H

#include <16nar/platform/memory/defs.h>

#include <16nar/platform/strings/static_name.h>

#include <vector>
#include <unordered_map>

namespace _16nar::memory
{

/// @brief Memory manager with multiple domains composed in tree-like hierarchy.
class NARENGINE_PLATFORM_API MemoryManager
{
public:
     /// @brief Default constructor.
     MemoryManager();

     /// @brief Destructor.
     ~MemoryManager();

     /// @brief Set the default domain which will be used as a fallback.
     /// @param[in] domain default memory domain.
     void set_default_domain( MemoryDomainPtr domain );

     /// @brief Register memory domain.
     /// @throws std::runtime_error in case of errors.
     /// @param[in] domain memory domain.
     /// @param[in] policy policy identifier specifying how often the domain is reset, zero policy is ignored.
     /// @param[in] parent name of the parent domain to be associated with the current one.
     void register_domain( MemoryDomainPtr domain,
          std::uint32_t policy = 0, strings::StaticName parent = {} );

     /// @brief Get memory domain by name.
     /// @details If there is no domain with specified name,
     /// then the default memory domain will be used, if any.
     /// @param[in] name name of the domain.
     /// @return memory domain or nullptr if suitable domain is not found.
     MemoryDomain *get_domain( strings::StaticName name );

     /// @brief Reset all memory domains with specified policy.
     /// @param[in] policy policy identifier.
     void reset_with_policy( std::uint32_t policy );

private:
     MemoryManager( const MemoryManager& ) = delete;
     MemoryManager& operator=( const MemoryManager& ) = delete;

private:
     using PolicyVector = std::unordered_map< std::uint32_t, std::vector< MemoryDomain * > >;
     using NamesMap = std::unordered_map< strings::StaticName, MemoryDomain * >;

     PolicyVector policies_;                           ///< memory domains grouped with policy identifier.
     NamesMap names_;                                  ///< memory domain names.
     MemoryDomainPtr default_domain_;                  ///< default memory domain.
     std::vector< MemoryDomainPtr > root_domains_;     ///< memory domains which have no parents.
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_MEMORY_MANAGER_H
