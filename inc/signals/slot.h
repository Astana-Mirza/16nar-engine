#ifndef _16NAR_SLOT_H
#define _16NAR_SLOT_H

#include "../abstract/basic_slot.h"

namespace _16nar {

class Signallable;

template <typename SignalType, typename Handler>
class Slot : public BasicSlot {
public:
	Slot(const Handler& hn) : handler{hn} {}
	void accept_signal(const Signal& sig) {
		handler(static_cast<const SignalType&>(sig));
	}

private:
	Handler handler;
};

}

#endif
