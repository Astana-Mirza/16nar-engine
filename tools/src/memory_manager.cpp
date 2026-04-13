#include <16nar/tools/memory_manager.h>

#include <16nar/tools/memory_domain.h>

#include <stdexcept>

namespace _16nar::tools
{

void MemoryManager::set_default_domain( MemoryDomainUniquePtr domain )
{
     default_domain_ = std::move( domain );
}


void MemoryManager::register_domain( MemoryDomainUniquePtr domain,
     std::uint32_t policy, StaticName parent )
{
     MemoryDomain *parent_domain{};
     if ( !parent.empty() )
     {
          const auto iter = names_.find( parent );
          if ( iter == names_.cend() )
          {
               throw std::runtime_error{ "Cannot find parent memory domain while registering '"
                    + std::string{ domain->get_name() } + "'" };
          }
          parent_domain = iter->second;
     }

     const StaticName name{ domain->get_name() };
     if ( !names_.insert( { name, domain.get() } ).second )
     {
          throw std::runtime_error{ "Cannot add memory domain '"
               + std::string{ domain->get_name() } + "' - duplicate or hash collision" };
     }

     if ( policy )
     {
          policies_[ policy ].emplace_back( domain.get() );
     }

     if ( parent_domain )
     {
          parent_domain->add_child( std::move( domain ) );
     }
     else
     {
          root_domains_.emplace_back( std::move( domain ) );
     }
}


MemoryDomain *MemoryManager::get_domain( StaticName name )
{
     const auto iter = names_.find( name );
     if ( iter != names_.cend() )
     {
          return iter->second;
     }
     return default_domain_.get();
}


void MemoryManager::reset_with_policy( std::uint32_t policy )
{
     const auto iter = policies_.find( policy );
     if ( iter == policies_.cend() )
     {
          return;
     }
     for ( auto ptr : iter->second )
     {
          ptr->reset();
     }
}

} // namespace _16nar::tools
