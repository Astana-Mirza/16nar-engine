/// @file
/// @brief File with MemoryManager class definition.
#ifndef _16NAR_PLATFORM_MEMORY_MEMORY_MANAGER_H
#define _16NAR_PLATFORM_MEMORY_MEMORY_MANAGER_H

#include <16nar/platform/memory/defs.h>

#include <16nar/platform/strings/name_table.h>

#include <vector>
#include <unordered_map>

namespace _16nar::memory
{

/// @brief Memory manager with multiple domains composed in tree-like hierarchy.
class NARENGINE_PLATFORM_API MemoryManager
{
public:
     /// @brief Constructor.
     /// @param[in] root_domain root memory domain.
     MemoryManager( IMemoryDomain& root_domain );

     /// @brief Destructor.
     ~MemoryManager();

     /// @brief Create memory domain.
     /// @tparam DomainType type of memory domain.
     /// @tparam Args types of domain constructor parameters.
     /// @param[in] name name of domain.
     /// @param[in] parent name of the parent domain to be
     /// associated with the current one, may be empty.
     /// @param[in] args arguments of domain constructor,
     /// except parent resource pointer (first parameter).
     /// @return Pointer to created domain, nullptr on error.
     template < typename DomainType, typename... Args >
     IMemoryDomain *add_domain( std::string_view name, strings::StaticName parent, Args... args )
     {
          Node *parent_node{};
          IMemoryDomain *parent_domain{ &root_domain_ };
          if ( !parent.empty() )
          {
               parent_node = get_node( parent );
               if ( !parent_node )
               {
                    return nullptr;
               }
          }

          const auto key = names_table_.add_name( name );
          if ( key.empty() )
          {
               return nullptr;
          }

          std::pmr::polymorphic_allocator< DomainType > alloc{ &root_domain_.get_resource() };
          auto *domain = alloc.allocate( 1 );
          try
          {
               alloc.construct( domain, &parent_domain->get_resource(), args... );
          }
          catch( ... )
          {
               alloc.deallocate( domain, 1 );
               throw;
          }
          add_domain_impl( key, domain, parent_node );
          return domain;
     }

     /// @brief Get memory domain by name.
     /// @param[in] name name of the domain.
     /// @param[in] strict strict match by name. If set to false,
     /// then root domain will be returned if the requested one is not found.
     /// @return memory domain or nullptr if suitable domain is not found.
     IMemoryDomain *get_domain( strings::StaticName name, bool strict = false );

     /// @brief Reset memory of a domain and all its children.
     /// @param[in] name name of a domain to be reset.
     void reset_domain( strings::StaticName name );

private:
     MemoryManager( const MemoryManager& ) = delete;
     MemoryManager& operator=( const MemoryManager& ) = delete;
     struct Node;

     /// @brief Get memory domain tree node.
     /// @param[in] name name of a domain.
     /// @return memory node, nullptr if the node is not found.
     Node *get_node( strings::StaticName name );

     /// @brief Add memory domain to memory domain tree.
     /// @param[in] name name of domain.
     /// @param[in] domain new allocated domain.
     /// @param[in] parent_node parent node of current node, nullptr if parent is root.
     void add_domain_impl( strings::StaticName name, IMemoryDomain *domain, Node *parent_node );

     /// @brief Implementation of recursive reset.
     /// @param[in] node node to be reset with all its children.
     /// @param[in] with_delete delete children of nodes after reset.
     void do_reset( Node& node, bool with_delete );

     /// @brief Custom deleter for polymorphic memory resource management.
     /// @tparam T type of managed object.
     template < typename T >
     struct Deleter
     {
          /// @brief Type of deleter function.
          using DeleterFunc = void ( * )( std::pmr::memory_resource *, T * );

          std::pmr::memory_resource *resource;    ///< memory resource owning the object.
          DeleterFunc deleter;                    ///< deleter function.

          /// @brief Function call operator.
          /// @param[in] ptr pointer to deleted object.
          void operator()( T *ptr ) const
          {
               deleter( resource, ptr );
          }
     };

     /// @brief Deleter of memory domain.
     using DomainDeleter = Deleter< IMemoryDomain >;

     /// @brief Deleter of node of memory domain tree.
     using NodeDeleter = Deleter< Node >;

     /// @brief Pointer to node of a memory domains tree.
     using NodePtr = std::unique_ptr< Node, NodeDeleter >;

     /// @brief Pointer to memory domain with exclusive ownership.
     using IMemoryDomainPtr = std::unique_ptr< IMemoryDomain, DomainDeleter >;

     /// @brief Memory domain node names.
     using NamesMap = std::pmr::unordered_map< strings::StaticName, Node * >;

     /// @brief Node of memory domain tree.
     struct Node
     {
          /// @brief Constructor.
          /// @param[in] domain_ptr memory domain.
          /// @param[in] resource memory resource for children vector.
          Node( IMemoryDomainPtr domain_ptr, std::pmr::memory_resource *resource );

          IMemoryDomainPtr domain;                ///< memory domain.

          // store not values but pointers because we need stable pointers in NamesMap
          std::pmr::vector< NodePtr > children;   ///< children of current node.
     };

private:
     NamesMap names_;                   ///< memory domain tree nodes with their names.
     std::pmr::vector< NodePtr > tree_; ///< children of root domain.
     strings::NameTable names_table_;   ///< table of memory domain string names.
     IMemoryDomain& root_domain_;       ///< root memory domain for all allocations.
};

} // namespace _16nar::memory

#endif // #ifndef _16NAR_PLATFORM_MEMORY_MEMORY_MANAGER_H
