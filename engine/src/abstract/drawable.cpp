#include "../../inc/abstract/drawable.h"

using namespace _16nar;

bool Drawable::is_visible() const {
	return visible;
}

void Drawable::set_visible(bool set) {
	visible = set;
}
