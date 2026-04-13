#include <16nar/tools/memory_domain.h>

namespace _16nar::tools
{

MemoryDomain::MemoryDomain( std::string_view name ):
     name_{ name.data(), name.size() }, children_{}
{}


MemoryDomain::~MemoryDomain()
{
     // destruct children before current memory_resource destruction
     children_.clear();
}


std::optional< std::uint64_t > MemoryDomain::get_usage() const
{
     return std::nullopt;
}


std::string_view MemoryDomain::get_name() const noexcept
{
     return std::string_view{ name_.c_str(), name_.size() };
}


void MemoryDomain::reset()
{
     for ( const auto& ptr : children_ )
     {
          ptr->reset();
     }
     do_reset();
}


void MemoryDomain::add_child( MemoryDomainUniquePtr child )
{
     children_.emplace_back( std::move( child ) );
}

} // namespace _16nar::tools
