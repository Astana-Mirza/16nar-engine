#include "../../inc/constructor/node_2d.h"

using namespace _16nar;

const Vector2f& Node2D::get_position() const {
	return transformer.getPosition();
}

float Node2D::get_rotation() const {
	return transformer.getRotation();
}

const Vector2f& Node2D::get_scale() const {
	return transformer.getScale();
}

const Vector2f& Node2D::get_origin() const {
	return transformer.getOrigin();
}

const Transform& Node2D::get_transform() const {
	return transformer.getTransform();
}

const Transform& Node2D::get_inverse_transform() const {
	return transformer.getInverseTransform();
}

void Node2D::set_position(float x, float y) {
	transformer.setPosition(x, y);
}

void Node2D::set_position(const Vector2f& position) {
	transformer.setPosition(position);
}

void Node2D::set_rotation(float angle) {
	transformer.setRotation(angle);
}

void Node2D::set_scale(float factor_x, float factor_y) {
	transformer.setScale(factor_x, factor_y);
}

void Node2D::set_scale(const Vector2f& factors) {
	transformer.setScale(factors);
}

void Node2D::set_origin(float x, float y) {
	transformer.setOrigin(x, y);
}

void Node2D::set_origin(const Vector2f& origin) {
	transformer.setOrigin(origin);
}

void Node2D::move(float offset_x, float offset_y) {
	transformer.move(offset_x, offset_y);
}

void Node2D::move(const Vector2f& offset) {
	transformer.move(offset);
}

void Node2D::rotate(float angle) {
	transformer.rotate(angle);
}

void Node2D::scale(float factor_x, float factor_y) {
	transformer.scale(factor_x, factor_y);
}

void Node2D::scale(const Vector2f& factor) {
	transformer.scale(factor);
}
