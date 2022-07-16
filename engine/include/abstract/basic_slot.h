/// @file
/// Header file with BasicSlot class definition.
#ifndef _16NAR_BASIC_SLOT_H
#define _16NAR_BASIC_SLOT_H

#include <abstract/signal.h>

namespace _16nar
{

/// Abstract base class for signal accepting slot
class BasicSlot
{
public:
     /// Accept an emitted signal.
     /// @param sig signal being accepted.
     virtual void accept_signal( const Signal& sig ) = 0;
     virtual ~BasicSlot()                            = default;
};

} // namespace _16nar

#endif // #ifndef _16NAR_BASIC_SLOT_H
