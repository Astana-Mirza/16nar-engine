#include "quadrant.h"

using namespace _16nar;

explicit Quadrant::Quadrant(const FloatRect& a) : area{a} {}

Quadrant::~Quadrant() {
	for (auto quad : children)
		delete quad;
}

/************************************************************************
** Get rectangle of this quadrant
*************************************************************************/
const FloatRect& Quadrant::get_area() const {
	return area;
}

/************************************************************************
** Get all 4 children of this quadrant
*************************************************************************/
const std::array<QuadPtr, 4>& Quadrant::get_children() const {
	return children;
}

/************************************************************************
** Get a parent quadrant in a quadrant tree
*************************************************************************/
const Quadrant *Quadrant::get_parent() const {
	return parent;
}

/************************************************************************
** Add a child quadrant to index idx
*************************************************************************/
void Quadrant::add_child(Quadrant* child, int idx) {
	child->parent = this;
	children[idx] = child;
}

/************************************************************************
** Add a drawable child to the quadrant
*************************************************************************/
void Quadrant::add_draw_child(Drawable *child) {
	draw_layers[child->layer].insert(child);
}

/************************************************************************
** Delete a drawable child from the quadrant
*************************************************************************/
void Quadrant::delete_draw_child(Drawable *child) {
	for (auto it = draw_layers[child->layer].begin();
			it != draw_layers[child->layer].end(); ++it) {
		if (*it == child) {
			draw_layers[child->layer].erase(it);
			if (draw_layers[child->layer].empty())	// erase the list
				draw_layers.erase(child->layer);
			return;
		}
	}
}

/************************************************************************
** Collect all objects that are in visible quadrants (recursive)
*************************************************************************/
void Quadrant::find_objects(RenderTarget& target, IntRect tr_area,
		std::map<int, std::set<DrawComponent *>>& layers) const {
	IntRect mapped(target.mapCoordsToPixel({area.left, area.top}),
				{(int)area.width, (int)area.height});
	if (mapped.intersects(tr_area)) {
		for (const auto& [lay, objs] : draw_layers) {
			layers[lay].insert(layers[lay].end(),
				objs.cbegin(), objs.cend());
		}
		for (const auto ptr : children) {
			if (ptr)
				ptr->find_objects(target, tr_area, layers);
		}
	}
}

/************************************************************************
** Draw all visible layers of objects
*************************************************************************/
void Quadrant::draw(RenderTarget& target, RenderStates states) const {
	std::map<int, std::set<DrawComponent *>> layers;
	find_objects(target, target.getViewport(target.getView()), layers);
	for (const auto& [lay, objs] : layers) {
		for (const auto ptr : objs) {
			if (ptr->is_visible())
				ptr->draw(target, states);
		}
	}
}
