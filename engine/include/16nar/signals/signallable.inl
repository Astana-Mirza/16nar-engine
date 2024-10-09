#ifndef _16NAR_SIGNALLABLE_INL
#define _16NAR_SIGNALLABLE_INL

namespace _16nar
{

template < typename SignalType, typename Handler >
void Signallable::connect( Signallable *sender, Handler&& handle )
{
     SlotId id{ std::type_index( typeid( SignalType ) ), sender };
     auto new_slot = std::make_unique< Slot< SignalType, Handler > >{ handle };
     auto new_slot_raw = new_slot.get();
     slots_[ id ].swap( new_slot );
     id.second->acceptors_[ id.first ][ this ] = new_slot_raw;
}


template < typename SignalType >
void Signallable::disconnect( Signallable *sender )
{
     SlotId id{ std::type_index( typeid( SignalType ) ), sender };
     slots_.erase( id );
     id.second->acceptors_[ id.first ].erase( this );
}


template < typename SignalType >
void Signallable::emit( const SignalType& sig )
{
     auto iter = acceptors_.find( std::type_index( typeid( SignalType ) ) );
     if ( iter == acceptors_.cend() )
     {
          return;
     }
     for ( auto& pair : iter->second )
     {
          pair.second->accept_signal( sig );
     }
}

} // namespace _16nar

#endif // #ifndef _16NAR_SIGNALLABLE_INL
