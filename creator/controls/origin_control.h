#ifndef ORIGINCONTROL_H
#define ORIGINCONTROL_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class OriginControl : public QGraphicsPixmapItem {
public:
	OriginControl(QGraphicsItem *parent);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ORIGINCONTROL_H
