#include "scale_control.h"

ScaleControl::ScaleControl(Type t, QGraphicsItem *parent):
QGraphicsPixmapItem{QPixmap{":/rsrc/scale_mark.png"}, parent},
node{dynamic_cast<AbstractNode *>(parent)}, type{t} {
	setFlags(QGraphicsItem::ItemIsSelectable);
	setOffset(-boundingRect().width()/2,
		-boundingRect().height()/2);
	hide();
}

void ScaleControl::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton) {
		setSelected(true);
	}
}

void ScaleControl::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	if (isSelected()) {
		auto new_pos = parentItem()->mapFromScene(event->scenePos());
		float factor_x = new_pos.x()
			/ parentItem()->boundingRect().width();
		float factor_y = new_pos.y()
			/ parentItem()->boundingRect().height();
		switch (type) {
		case Down:
			if (abs(factor_y) > 0.01)
				node->scale(1, factor_y);
			break;
		case Corner:
			if (abs(factor_y) > 0.01 && abs(factor_x) > 0.01)
				node->scale(factor_x, factor_y);
			break;
		case Right:
			if (abs(factor_x) > 0.01)
				node->scale(factor_x, 1);
			break;
		}
	}
}

void ScaleControl::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::MouseButton::LeftButton) {
		setSelected(false);
	}
}
