#include "../../../inc/constructor/abstract/drawable_node.h"

using namespace _16nar;

DrawableNode::DrawableNode(Quadrant *q) : quad{q} {
	quad->add_draw_child(this);
}

DrawableNode::~DrawableNode() {
	quad->delete_draw_child(this);
}

void DrawableNode::fix_quadrant() {
	bool found;
	Quadrant *prev = quad;
	while (quad->get_parent() && !check_quadrant(quad)) {
		quad = quad->get_parent();
	}
	do {	// go to the lowest possible level
		const auto& ch = quad->get_children();
		found = false;
		for (size_t i = 0; i < _16NAR_QUAD_COUNT && !found; i++) {
			if (ch[i] && check_quadrant(ch[i])) {
				quad = ch[i];
				found = true;
			}
		}
	} while (found);
	if (prev != quad) {
		prev->delete_draw_child(this);
		quad->add_draw_child(this);
	}
}

void DrawableNode::loop_call(bool upd, float delta) {
	loop(delta);
	upd = upd || transformed;
	transformed = false;
	if (upd)
		fix_quadrant();
	for (auto ch : get_children())
		ch->loop_call(upd, delta);
}
