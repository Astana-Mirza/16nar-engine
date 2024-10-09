/// @file
/// @brief Header file with Signallable class definition.
#ifndef _16NAR_SIGNALLABLE_H
#define _16NAR_SIGNALLABLE_H

#include <16nar/signals/slot.h>

#include <unordered_map>
#include <memory>
#include <typeindex>

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
/// @details Acts as both sender and receiver of signals.
class ENGINE_API Signallable
{
public:
     using SlotId = std::pair< std::type_index, Signallable * >;

     /// @brief Sets a handler for a signal of given type from specified sender.
     /// @tparam SignalType type of a signal being handled.
     /// @tparam Handler type of a handler, must be callable.
     /// @param[in] sender pointer to an object which emits signals.
     /// @param[in] handle handler for the signal.
     template < typename SignalType, typename Handler >
     void connect( Signallable *sender, Handler&& handle );

     /// @brief Deletes a handler for a signal of given type from specified sender.
     /// @tparam SignalType type of a signal being handled.
     /// @param[in] sender pointer to an object which emits signals.
     template < typename SignalType >
     void disconnect( Signallable *sender );

     /// @brief Emits a signal, making all acceptors to handle it
     /// @tparam signalType type of a signal being emitted.
     /// @param[in] sig signal object.
	template < typename SignalType >
	void emit( const SignalType& sig );

     /// @brief Destructor, disconnects all slots.
     virtual ~Signallable();

private:
     using AcceptorMap = std::unordered_map< Signallable *, BasicSlot * >;
     using SignalMap = std::unordered_map< std::type_index, AcceptorMap >;
     using SlotMap = std::unordered_map< SlotId, std::unique_ptr< BasicSlot > >;

     SignalMap acceptors_;    ///< all slots accepting signals from this object.
                              ///< Maps signal type and receiver to accepting slot,
                              ///< has ability to group by signal type.
     SlotMap slots_;          ///< maps slot identifier to accepting slot.
};

} // namespace _16nar

#include <16nar/signals/signallable.inl>

#endif // #ifndef _16NAR_SIGNALLABLE_H
