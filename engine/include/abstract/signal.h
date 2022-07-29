/// @file
/// Header file with Signal abstract class definition.
#ifndef _16NAR_SIGNAL_H
#define _16NAR_SIGNAL_H

#include <16nardefs.h>

namespace _16nar
{

/// Abstract base class for a signal.
struct ENGINE_API Signal
{
     virtual ~Signal() = default;
};

} // namespace _16nar

#endif // #ifndef _16NAR_SIGNAL_H
