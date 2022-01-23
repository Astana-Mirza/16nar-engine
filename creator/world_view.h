#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class WorldView : public QGraphicsView {
public:
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
};

#endif // WORLDVIEW_H
