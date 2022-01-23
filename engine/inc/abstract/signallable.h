#ifndef _16NAR_SIGNALLABLE_H
#define _16NAR_SIGNALLABLE_H

#include <unordered_map>
#include <typeindex>
#include <utility>
#include "../signals/slot.h"

namespace _16nar { class Signallable; }

namespace std {
	template<>
	struct hash<std::pair<std::type_index, _16nar::Signallable*>> {
		size_t operator()(const std::pair<std::type_index,
					_16nar::Signallable*>& id) const;
	};
}

namespace _16nar {

class Signallable {
public:
	using SlotId = std::pair<std::type_index, Signallable*>;

	template <typename SignalType, typename Handler>
	void connect(Signallable *sender, const Handler& handle) {
		SlotId id{std::type_index(typeid(SignalType)), sender};
		auto slot = new Slot<SignalType, Handler>{handle};
		delete slots[id]; // free old slot if it exists
		slots[id] = slot;
		id.second->acceptors[id.first][this] = slot;
	}

	template <typename SignalType>
	void disconnect(Signallable *sender) {
		SlotId id{std::type_index(typeid(SignalType)), sender};
		delete slots[id];
		slots.erase(id);
		id.second->acceptors[id.first].erase(this);
	}

	template <typename SignalType>
	void emit(const SignalType& sig) {
		auto& handlers = acceptors[std::type_index(typeid(SignalType))];
		for (auto& pair : handlers)
			pair.second->accept_signal(sig);
	}

	virtual ~Signallable();

private:
	// map signal_type and receiver to accepting slot
	// has ability to group by signal type
	std::unordered_map<
		std::type_index, std::unordered_map<
			Signallable*, BasicSlot*>> acceptors;
	// map pair {signal_type, sender} to accepting slot
	std::unordered_map<SlotId, BasicSlot*> slots;
};

}

#endif
