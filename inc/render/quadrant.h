#ifndef _16NAR_QUADRANT_H
#define _16NAR_QUADRANT_H

#include <map>
#include <array>
#include <set>
#include "../16nardefs.h"
#include "../abstract/drawable.h"

namespace _16nar {

class Drawable;

// A class for space partitioning
class Quadrant : public sf::Drawable {
	public:
		explicit Quadrant(const FloatRect& a);
		~Quadrant();
		const FloatRect& get_area() const;
		const std::array<Quadrant*, 4>& get_children() const;
		const Quadrant *get_parent() const;
		void add_child(Quadrant* child, int idx);
		void add_draw_child(Drawable *child);
		void delete_draw_child(Drawable *child);
		void find_objects(RenderTarget& target, IntRect tr_area,
			 std::map<int, std::set<Drawable *>>& layers) const;
		void draw(RenderTarget& target, RenderStates states) const;

	private:
		Quadrant *parent = nullptr;
		FloatRect area;
		std::array<Quadrant*, 4> children = {nullptr, nullptr,
						nullptr, nullptr};
		std::map<int, std::set<Drawable *>> draw_layers;
};


}

#endif // #ifndef _16NAR_QUADRANT_H
