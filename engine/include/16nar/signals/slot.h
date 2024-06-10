/// @file
/// @brief Header file with Slot template class definition.
#ifndef _16NAR_SLOT_H
#define _16NAR_SLOT_H

#include <16nar/signals/basic_slot.h>

namespace _16nar
{

/// @brief Class for signal accepting slot with custom handler.
template < typename SignalType, typename Handler >
class Slot : public BasicSlot
{
public:
     /// @brief Constructor, taking handler.
     /// @param[in] handler Handler of a signal.
     Slot( const Handler& handler ) : handler_{ handler } {}


     /// @brief Accept an emitted signal.
     /// @param[in] sig Signal being accepted.
     void accept_signal( const Signal& sig ) override
     {
          handler_( static_cast< const SignalType& >( sig ) );
     }

private:
     Handler handler_;        ///< handler of a signal, callable.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SLOT_H
