#include "sprite_node.h"

SpriteNode::SpriteNode(const QString& str) {
	setPixmap(QPixmap(str));
}
