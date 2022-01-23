#include "world_view.h"

void WorldView::wheelEvent(QWheelEvent *event) {
	int num_steps = event->angleDelta().y() / 8;
	if (num_steps > 0) {
		scale(1.25, 1.25);
	} else if (num_steps < 0) {
		scale(0.8, 0.8);
	}
	event->accept();
}

void WorldView::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Shift) {
		setDragMode(QGraphicsView::RubberBandDrag);
	}
}

void WorldView::keyReleaseEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Shift) {
		setDragMode(QGraphicsView::ScrollHandDrag);
	}
}
