#ifndef _16NAR_DRAWABLE_NODE_H
#define _16NAR_DRAWABLE_NODE_H

#include "../node.h"
#include "../../abstract/drawable.h"

class DrawableNode : public Node, public Drawable {
public:
	void set_position(float x, float y);
	void set_position(const Vector2f& position);
	void set_rotation(float angle);
	void set_scale(float factor_x, float factor_y);
	void set_scale(const Vector2f& factors);
	void set_origin(float x, float y);
	void set_origin(const Vector2f& origin);
	void move(float offset_x, float offset_y);
	void move(const Vector2f& offset);
	void rotate(float angle);
	void scale(float factor_x, float factor_y);
	void scale(const Vector2f& factor);
};

#endif
