#include <abstract/signallable.h>

namespace std
{

     size_t hash< _16nar::Signallable::SlotId >::operator()( const _16nar::Signallable::SlotId& id ) const
     {
          return hash< type_index >{}( id.first ) + hash< _16nar::Signallable * >{}( id.second );
	}

} // namespace std


namespace _16nar
{

Signallable::~Signallable()
{
     for ( auto& sig_types : acceptors_ )
     {
          for ( auto& pair : sig_types.second )
          {
               delete pair.second;
               pair.first->slots_.erase( SlotId{ sig_types.first, this } );
          }
     }
     for ( auto& pair : slots_ )
     {
          delete pair.second;
          auto& id = pair.first;
          id.second->acceptors_[ id.first ].erase( this );
     }
}

} // namespace _16nar
