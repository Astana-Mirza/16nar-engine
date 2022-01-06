#include "../../inc/abstract/signallable.h"

using namespace _16nar;

namespace std {
	size_t hash<Signallable::SlotId>::operator()(const
				Signallable::SlotId& id) const {
		return hash<type_index>{}(id.first) +
			hash<Signallable*>{}(id.second);
	}
}

Signallable::~Signallable() {
	for (auto& sig_types : acceptors) {
		for (auto& pair : sig_types.second) {
			delete pair.second;
			pair.first->slots.erase(
				SlotId{sig_types.first, this});
		}
	}
	for (auto& pair : slots) {
		delete pair.second;
		auto& id = pair.first;
		id.second->acceptors[id.first].erase(this);
	}
}
