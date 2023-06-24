/// @file
/// @brief Header file with BasicSlot class definition.
#ifndef _16NAR_BASIC_SLOT_H
#define _16NAR_BASIC_SLOT_H

#include <16nar/abstract/signal.h>

namespace _16nar
{

/// @brief Abstract base class for signal accepting slot
class ENGINE_API BasicSlot
{
public:
     /// @brief Accept an emitted signal.
     /// @param[in] sig signal being accepted.
     virtual void accept_signal( const Signal& sig ) = 0;
     virtual ~BasicSlot()                            = default;
};

} // namespace _16nar

#endif // #ifndef _16NAR_BASIC_SLOT_H
