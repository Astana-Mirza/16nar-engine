/// @file
/// @brief File with MemoryDomain abstract class definition.
#ifndef _16NAR_TOOLS_MEMORY_DOMAIN_H
#define _16NAR_TOOLS_MEMORY_DOMAIN_H

#include <16nar/tools/defs.h>

#include <string>
#include <optional>
#include <vector>
#include <memory_resource>

namespace _16nar::tools
{

/// @brief Abstract memory domain having its own memory resource.
class ENGINE_API MemoryDomain
{
public:
     /// @brief Constructor.
     /// @param[in] name name of the memory domain.
     MemoryDomain( std::string_view name );

     /// @brief Destructor.
     virtual ~MemoryDomain();

     /// @brief Get memory resource of the domain.
     /// @return memory resource of the domain.
     virtual std::pmr::memory_resource& get_resource() = 0; 

     /// @brief Get usage of the memory domain, in bytes.
     /// @return usage of the memory domain, in bytes, may be undefined.
     virtual std::optional< std::uint64_t > get_usage() const;

     /// @brief Get name otf the domain.
     /// @return name of the domain.
     std::string_view get_name() const noexcept;

     /// @brief Reset the memory domain and release all the memory, if possible.
     /// @details Children domains will be reset before the parent domain.
     void reset();

     /// @brief Add child memory domain.
     /// @param[in] child child memory domain.
     void add_child( MemoryDomainUniquePtr child );

protected:
     /// @brief Implementation of current domain reset.
     virtual void do_reset() = 0;

private:
     std::string name_;                                ///< name of the domain.
     std::vector< MemoryDomainUniquePtr > children_;   ///< children domains.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_MEMORY_DOMAIN_H
