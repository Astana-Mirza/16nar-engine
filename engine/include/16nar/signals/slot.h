/// @file
/// @brief Header file with Slot template class definition.
#ifndef _16NAR_SLOT_H
#define _16NAR_SLOT_H

#include <16nar/signals/basic_slot.h>
#include <16nar/signals/signal.h>
#include <16nar/logger/logger.h>

#include <type_traits>

namespace _16nar
{

/// @brief Class for signal accepting slot with custom handler.
/// @tparam SignalType type of a signal.
/// @tparam Handler type of signal handler, must be invocable with const SignalType&.
template < typename SignalType, typename Handler >
class Slot : public BasicSlot
{
public:
     static_assert( std::is_base_of_v< Signal, std::decay_t< SignalType > >,
          "SignalType must be derived from Signal" );
     static_assert( std::is_invocable_v< Handler, const SignalType& >,
          "Handler must be invocable with SignalType" );

     /// @brief Constructor, taking handler.
     /// @param[in] handler handler of a signal.
     Slot( Handler&& handler ) : handler_{ std::forward< Handler >( handler ) } {}


     /// @brief Accept an emitted signal.
     /// @param[in] sig signal being accepted.
     void accept_signal( const Signal& sig ) override
     {
          auto *casted_sig = dynamic_cast< const SignalType * >( &sig );
          if ( !casted_sig )
          {
               LOG_16NAR_ERROR( "Cannot handle signal in slot, bad cast" );
               return;
          }
          handler_( *casted_sig );
     }

private:
     Handler handler_;        ///< handler of a signal, callable.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SLOT_H
