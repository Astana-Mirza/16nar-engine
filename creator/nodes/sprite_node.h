#ifndef SPRITENODE_H
#define SPRITENODE_H

#include <QGraphicsPixmapItem>
#include "node.h"

class SpriteNode : public Node<QGraphicsPixmapItem> {
public:
	SpriteNode(const QString& str);
};

#endif // SPRITENODE_H
