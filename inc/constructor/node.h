#ifndef _16NAR_NODE_H
#define _16NAR_NODE_H

#include <set>
#include <logic_error>
#include "../16nardefs.h"

namespace _16nar {

class Node {
public:
	virtual ~Node();
	virtual void setup() {}
	virtual void update() {}
	void set_parent(const Node *par);
	const Node *get_parent() const;
	const std::set<Node *>& get_children() const;

	const Vector2f& get_position() const;
	float get_rotation() const;
	const Vector2f& get_scale() const;
	const Vector2f& get_origin() const;
	const Transform& get_transform() const;
	const Transform& get_inverse_transform() const;

	virtual void set_position(float x, float y);
	virtual void set_position(const Vector2f& position);
	virtual void set_rotation(float angle);
	virtual void set_scale(float factor_x, float factor_y);
	virtual void set_scale(const Vector2f& factors);
	virtual void set_origin(float x, float y);
	virtual void set_origin(const Vector2f& origin);
	virtual void move(float offset_x, float offset_y);
	virtual void move(const Vector2f& offset);
	virtual void rotate(float angle);
	virtual void scale(float factor_x, float factor_y);
	virtual void scale(const Vector2f& factor);

private:
	void add_child(const Node *ch);
	void remove_child(const Node *ch);

	Transformer transformer;
	std::set<Node *> children;
	Node *parent = nullptr;
};

}

#endif
