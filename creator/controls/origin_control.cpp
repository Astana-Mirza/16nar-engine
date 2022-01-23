#include "origin_control.h"

OriginControl::OriginControl(QGraphicsItem *parent):
	QGraphicsPixmapItem{QPixmap{":/rsrc/origin_mark.png"}, parent} {
	setFlags(QGraphicsItem::ItemIsSelectable);
	setOffset(-boundingRect().width()/2,
		-boundingRect().height()/2);
	setPos(parentItem()->transformOriginPoint());
	hide();
}

void OriginControl::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton) {
		setSelected(true);
	}
}

void OriginControl::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	if (isSelected()) {
		parentItem()->setRotation(0);
		setPos(parentItem()->mapFromScene(event->scenePos()));
		parentItem()->setTransformOriginPoint(pos());
	}
}

void OriginControl::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton) {
		setSelected(false);
	}
}
