#include "../../inc/constructor/node.h"

Node::~Node() {
	for (auto child : children)
		delete child;
}

void Node::set_parent(const Node *par) {
	if (!par)
		throw std::logic_error{"parent is nullptr"};
	if (parent)
		parent->remove_child(this);
	parent = par;
	parent->add_child(this);
}

const Node *Node::get_parent() const {
	return parent;
}

const std::set<Node *>& Node::get_children() const {
	return children;
}

void Node::add_child(const Node *ch) {
	children.insert(ch);
}

void Node::remove_child(const Node *ch) {
	children.remove(ch);
}

const Vector2f& Node::get_position() const {
	return transformer.getPosition();
}

float Node::get_rotation() const {
	return transformer.getRotation();
}

const Vector2f& Node::get_scale() const {
	return transformer.getScale();
}

const Vector2f& Node::get_origin() const {
	return transformer.getOrigin();
}

const Transform& Node::get_transform() const {
	return transformer.getTransform();
}

const Transform& Node::get_inverse_transform() const {
	return transformer.getInverseTransform();
}

void Node::set_position(float x, float y) {
	transformer.setPosition(x, y);
}

void Node::set_position(const Vector2f& position) {
	transformer.setPosition(position);
}

void Node::set_rotation(float angle) {
	transformer.setRotation(angle);
}

void Node::set_scale(float factor_x, float factor_y) {
	transformer.setScale(factor_x, factor_y);
}

void Node::set_scale(const Vector2f& factors) {
	transformer.setScale(factors);
}

void Node::set_origin(float x, float y) {
	transformer.setOrigin(x, y);
}

void Node::set_origin(const Vector2f& origin) {
	transformer.setOrigin(origin);
}

void Node::move(float offset_x, float offset_y) {
	transformer.move(offset_x, offset_y);
}

void Node::move(const Vector2f& offset) {
	transformer.move(offset);
}

void Node::rotate(float angle) {
	transformer.rotate(angle);
}

void Node::scale(float factor_x, float factor_y) {
	transformer.scale(factor_x, factor_y);
}

void Node::scale(const Vector2f& factor) {
	transformer.scale(factor);
}
