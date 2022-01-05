#include "../../inc/constructor/sprite_node.h"

using namespace _16nar;

SpriteNode::SpriteNode(Quadrant *q, const Texture& tex):
	DrawableNode::DrawableNode(q), sprite(tex) {}

SpriteNode::SpriteNode(Quadrant *q, const Texture& tex, const IntRect& rect):
	DrawableNode::DrawableNode(q), sprite(tex, rect) {}

void SpriteNode::draw(RenderTarget& target, RenderStates states) const {
	target.draw(sprite);
}

const Vector2f& SpriteNode::get_position() const {
	return sprite.getPosition();
}

float SpriteNode::get_rotation() const {
	return sprite.getRotation();
}

const Vector2f& SpriteNode::get_scale() const {
	return sprite.getScale();
}

const Vector2f& SpriteNode::get_origin() const {
	return sprite.getOrigin();
}

const Transform& SpriteNode::get_transform() const {
	return sprite.getTransform();
}

const Transform& SpriteNode::get_inverse_transform() const {
	return sprite.getInverseTransform();
}

void SpriteNode::set_position(float x, float y) {
	sprite.setPosition(x, y);
	fix_quadrant();
}

void SpriteNode::set_position(const Vector2f& position) {
	sprite.setPosition(position);
	fix_quadrant();
}

void SpriteNode::set_rotation(float angle) {
	sprite.setRotation(angle);
	fix_quadrant();
}

void SpriteNode::set_scale(float factor_x, float factor_y) {
	sprite.setScale(factor_x, factor_y);
	fix_quadrant();
}

void SpriteNode::set_scale(const Vector2f& factors) {
	sprite.setScale(factors);
	fix_quadrant();
}

void SpriteNode::set_origin(float x, float y) {
	sprite.setOrigin(x, y);
	fix_quadrant();
}

void SpriteNode::set_origin(const Vector2f& origin) {
	sprite.setOrigin(origin);
	fix_quadrant();
}

void SpriteNode::move(float offset_x, float offset_y) {
	sprite.move(offset_x, offset_y);
	fix_quadrant();
}

void SpriteNode::move(const Vector2f& offset) {
	sprite.move(offset);
	fix_quadrant();
}

void SpriteNode::rotate(float angle) {
	sprite.rotate(angle);
	fix_quadrant();
}

void SpriteNode::scale(float factor_x, float factor_y) {
	sprite.scale(factor_x, factor_y);
	fix_quadrant();
}

void SpriteNode::scale(const Vector2f& factor) {
	sprite.scale(factor);
	fix_quadrant();
}

bool SpriteNode::check_quadrant(const Quadrant *q) const {
	auto transform = get_global_transform();
	auto r = transform.transformRect(sprite.getLocalBounds());
	if (q->get_area().contains(r.left, r.top) &&
	    q->get_area().contains(r.left+r.width-1, r.top+r.height-1))
		return true;
	return false;
}
