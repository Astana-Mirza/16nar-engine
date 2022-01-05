#ifndef _16NAR_DRAWABLE_NODE_H
#define _16NAR_DRAWABLE_NODE_H

#include "node.h"
#include "../../abstract/drawable.h"

namespace _16nar {

class DrawableNode : public Node, public Drawable {
public:
	DrawableNode(Quadrant *q);
	~DrawableNode();
	void fix_quadrant();

protected:
	Quadrant *quad;

	virtual bool check_quadrant(const Quadrant *q) const =0;
};

}

#endif
