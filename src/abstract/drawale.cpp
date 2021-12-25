#include "../../inc/abstract/drawable.h"

bool is_visible() const {
	return visible;
}

void set_visible(bool set) {
	visible = set;
}
