/// @file
/// @brief Header file with Signallable class definition.
#ifndef _16NAR_SIGNALLABLE_H
#define _16NAR_SIGNALLABLE_H

#include <unordered_map>
#include <typeindex>

#include <16nar/signals/slot.h>

namespace _16nar
{
     class Signallable;
}


namespace std
{
     /// @brief Definition of hash for pair of std::type_index and pointer, needed for signals.
     template<>
     struct hash< std::pair< std::type_index, _16nar::Signallable * > >
     {
          size_t operator()( const std::pair< std::type_index, _16nar::Signallable * >& id ) const;
     };
}


namespace _16nar
{

/// @brief A base class which can send and accept signals.
/// Acts as both sender and receiver of signals.
class ENGINE_API Signallable
{
public:
     using SlotId = std::pair< std::type_index, Signallable * >;

     /// @brief Sets a handler for a signal of given type from specified sender.
     /// @tparam SignalType type of a signal being handled.
     /// @tparam Handler type of a handler, must be callable.
     /// @param sender pointer to an object which emits signals.
     /// @param handle handler for the signal.
     template < typename SignalType, typename Handler >
     void connect( Signallable *sender, const Handler& handle )
     {
          SlotId id{ std::type_index( typeid( SignalType ) ), sender };
          auto slot = new Slot< SignalType, Handler >{ handle };
          delete slots_[ id ]; // free old slot if it exists
          slots_[ id ] = slot;
          id.second->acceptors_[ id.first ][ this ] = slot;
	}


     /// @brief Deletes a handler for a signal of given type from specified sender.
     /// @tparam SignalType type of a signal being handled.
     /// @param sender pointer to an object which emits signals.
     template < typename SignalType >
     void disconnect( Signallable *sender )
     {
          SlotId id{ std::type_index( typeid( SignalType ) ), sender };
          delete slots_[ id ];
          slots_.erase( id );
          id.second->acceptors_[ id.first ].erase( this );
     }


     /// @brief Emits a signal, making all acceptors to handle it
     /// @tparam signalType type of a signal being emitted.
     /// @param sig signal object.
	template < typename SignalType >
	void emit( const SignalType& sig )
     {
          auto& handlers = acceptors_[ std::type_index( typeid( SignalType ) ) ];
          for ( auto& pair : handlers )
          {
               pair.second->accept_signal( sig );
          }
     }

     virtual ~Signallable();

private:
     using AcceptorMap = std::unordered_map< Signallable *, BasicSlot * >;
     std::unordered_map< std::type_index, AcceptorMap > acceptors_; ///< all slots accepting signals from this object.
                                                                    ///< maps signal type and receiver to accepting slot,
                                                                    ///< has ability to group by signal type.
     std::unordered_map< SlotId, BasicSlot * > slots_;              ///< maps slot identifier to accepting slot.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SIGNALLABLE_H
