/// @file
/// @brief Header file with Slot template class definition.
#ifndef _16NAR_SLOT_H
#define _16NAR_SLOT_H

#include <16nar/signals/basic_slot.h>
#include <16nar/logger/logger.h>

#include <stdexcept>

namespace _16nar
{

/// @brief Class for signal accepting slot with custom handler.
template < typename SignalType, typename Handler >
class Slot : public BasicSlot
{
public:
     /// @brief Constructor, taking handler.
     /// @param[in] handler Handler of a signal.
     Slot( Handler&& handler ) : handler_{ handler } {}


     /// @brief Accept an emitted signal.
     /// @param[in] sig Signal being accepted.
     void accept_signal( const Signal& sig ) override
     {
          try
          {
               handler_( dynamic_cast< const SignalType& >( sig ) );
          }
          catch ( const std::bad_cast& ex )
          {
               LOG_16NAR_ERROR( "Cannot handle signal in slot, bad cast: " << ex.what() );
          }
     }

private:
     Handler handler_;        ///< handler of a signal, callable.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SLOT_H
