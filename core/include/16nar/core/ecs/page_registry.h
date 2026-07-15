/// @file
/// @brief File with PageRegistry class definition.
#ifndef _16NAR_CORE_ECS_PAGE_REGISTRY_H
#define _16NAR_CORE_ECS_PAGE_REGISTRY_H

#include <16nar/core/ecs/defs.h>

#include <memory_resource>
#include <vector>

namespace _16nar::ecs
{

/// @brief Registry of available page identifiers.
class NARENGINE_CORE_API PageRegistry
{
public:
     /// @brief Constructor.
     /// @param[in] resource memory resource for utility data allocations.
     PageRegistry( std::pmr::memory_resource& resource );

     /// @brief Check if the registry is empty.
     /// @return true if the registry is empty, false otherwise.
     bool empty() const noexcept;

     /// @brief Add page to registry.
     /// @param[in] page identifier of the page.
     void add_page( EcsId page );

     /// @brief Add range of pages to registry.
     /// @param[in] first identifier of the first page.
     /// @param[in] last identifier of the last page (not included in the range).
     void add_page_range( EcsId first, EcsId last );

     /// @brief Pop most suitable page from the registry.
     void pop_page();

     /// @brief Get most suitable page from the registry.
     /// @return page identifier on success, -1 if registry is empty.
     EcsId get_top_page();

private:
     PageRegistry( const PageRegistry&& ) = delete;
     PageRegistry& operator=( const PageRegistry& ) = delete;

     std::pmr::vector< EcsId > pages_;  ///< identifiers of available pages.
};

} // namespace _16nar::ecs

#endif // #ifndef _16NAR_CORE_ECS_PAGE_REGISTRY_H

