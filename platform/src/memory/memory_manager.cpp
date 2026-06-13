#include <16nar/platform/memory/memory_manager.h>

#include <16nar/platform/memory/imemory_domain.h>

#include <stdexcept>

namespace _16nar::memory
{

MemoryManager::MemoryManager( IMemoryDomain& root_domain ):
     names_{ &root_domain.get_resource() },
     tree_{ &root_domain.get_resource() },
     names_table_{ root_domain },
     root_domain_{ root_domain }
{}


MemoryManager::~MemoryManager()
{
     for ( auto& ptr : tree_ )
     {
          do_reset( *ptr, true );
          ptr.reset();
     }
     names_.clear();
}


IMemoryDomain *MemoryManager::get_domain( strings::StaticName name, bool strict )
{
     auto *node = get_node( name );
     if ( !node )
     {
          return strict ? nullptr : &root_domain_;
     }
     return node->domain.get();
}


void MemoryManager::reset_domain( strings::StaticName name )
{
     auto *node = get_node( name );
     if ( !node )
     {
          return;
     }
     do_reset( *node, false );
}


MemoryManager::Node *MemoryManager::get_node( strings::StaticName name )
{
     const auto iter = names_.find( name );
     if ( iter == names_.cend() )
     {
          return nullptr;
     }
     return iter->second;
}


void MemoryManager::add_domain_impl( strings::StaticName name, IMemoryDomain *domain, MemoryManager::Node *parent_node )
{
     IMemoryDomainPtr domain_ptr{ domain, DomainDeleter{ &root_domain_.get_resource(),
          []( std::pmr::memory_resource *resource, IMemoryDomain *domain )
          {
               std::pmr::polymorphic_allocator< IMemoryDomain > alloc{ resource };
               alloc.destroy( domain );
               alloc.deallocate( domain, 1 );
          } }
     };

     std::pmr::polymorphic_allocator< Node > alloc{ &root_domain_.get_resource() };
     auto *node = alloc.allocate( 1 );
     try
     {
          alloc.construct( node, std::move( domain_ptr ), &root_domain_.get_resource() );
     }
     catch( ... )
     {
          alloc.deallocate( node, 1 );
          throw;
     }

     NodePtr node_ptr{ node, NodeDeleter{ &root_domain_.get_resource(),
          []( std::pmr::memory_resource *resource, Node *node )
          {
               std::pmr::polymorphic_allocator< Node > alloc{ resource };
               alloc.destroy( node );
               alloc.deallocate( node, 1 );
          } }
     };
     auto& parent_children = parent_node ? parent_node->children : tree_;
     parent_children.emplace_back( std::move( node_ptr ) );
     names_.emplace( name, node );
}


void MemoryManager::do_reset( MemoryManager::Node& node, bool with_delete )
{
     for ( auto& ptr : node.children )
     {
          do_reset( *ptr, with_delete );
     }

     if ( with_delete )
     {
          node.children.clear();
          node.domain.reset();
     }
     else
     {
          node.domain->reset();
     }
}


MemoryManager::Node::Node( MemoryManager::IMemoryDomainPtr domain_ptr,
     std::pmr::memory_resource *resource ):
     domain{ std::move( domain_ptr ) },
     children( resource )
{}

} // namespace _16nar::memory
