#ifndef SCALECONTROL_H
#define SCALECONTROL_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "../nodes/abstract_node.h"

class ScaleControl : public QGraphicsPixmapItem {
public:
	enum Type { Down, Right, Corner };
	ScaleControl(Type t, QGraphicsItem *parent);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	AbstractNode *node;
	Type type;
};

#endif // SCALECONTROL_H
