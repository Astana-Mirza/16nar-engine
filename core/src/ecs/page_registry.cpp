#include <16nar/core/ecs/page_registry.h>

#include <algorithm>
#include <limits>

namespace _16nar::ecs
{

PageRegistry::PageRegistry( std::pmr::memory_resource& resource ):
     pages_( &resource )
{}


bool PageRegistry::empty() const noexcept
{
     return pages_.empty();
}


void PageRegistry::add_page( EcsId page )
{
     pages_.emplace_back( page );
     std::push_heap( pages_.begin(), pages_.end(), std::greater< EcsId >{} );
}


void PageRegistry::add_page_range( EcsId first, EcsId last )
{
     if ( first >= last )
     {
          return;
     }
     for ( auto page = first; page < last; ++page )
     {
          pages_.emplace_back( page );
     }
     std::make_heap( pages_.begin(), pages_.end(), std::greater< EcsId >{} );
}


void PageRegistry::pop_page()
{
     if ( pages_.empty() )
     {
          return;
     }
     std::pop_heap( pages_.begin(), pages_.end(), std::greater< EcsId >{} );
     pages_.pop_back();
}


EcsId PageRegistry::get_top_page()
{
     if ( pages_.empty() )
     {
          return std::numeric_limits< EcsId >::max();
     }
     return pages_.front();
}

} // namespace _16nar::ecs
