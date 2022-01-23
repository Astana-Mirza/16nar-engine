#ifndef _16NAR_SPRITE_NODE_H
#define _16NAR_SPRITE_NODE_H

#include "abstract/drawable_node.h"

namespace _16nar {

class SpriteNode : public DrawableNode {
public:
	using DrawableNode::DrawableNode;
	SpriteNode(Quadrant *q, const Texture& tex);
	SpriteNode(Quadrant *q, const Texture& tex, const IntRect& rect);

	void draw(RenderTarget& target, RenderStates states) const;

	const Vector2f& get_position() const;
	float get_rotation() const;
	const Vector2f& get_scale() const;
	const Vector2f& get_origin() const;
	const Transform& get_transform() const;
	const Transform& get_inverse_transform() const;

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

private:
	Sprite sprite;

	bool check_quadrant(const Quadrant *q) const;
};

}

#endif
