#ifndef _16NAR_BASIC_SLOT_H
#define _16NAR_BASIC_SLOT_H

#include "signal.h"

namespace _16nar {

class BasicSlot {
public:
	virtual void accept_signal(const Signal& sig) =0;
	virtual ~BasicSlot() =default;
};

}

#endif
