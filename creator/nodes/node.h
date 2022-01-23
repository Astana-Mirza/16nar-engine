#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "../controls/origin_control.h"
#include "../controls/scale_control.h"
#include "abstract_node.h"

template <typename T>
class Node : public AbstractNode, public T {
public:
	Node() {
		T::setFlags(QGraphicsItem::ItemIsMovable
			| QGraphicsItem::ItemIsSelectable);
		or_con = new OriginControl(this);
		sc_con[0] = new ScaleControl(ScaleControl::Down, this);
		sc_con[1] = new ScaleControl(ScaleControl::Corner, this);
		sc_con[2] = new ScaleControl(ScaleControl::Right, this);
	}

	void scale(float factor_x, float factor_y) {
		QTransform tr = T::transform();
		tr.scale(factor_x, factor_y);
		T::setTransform(tr);
	}

	void mousePressEvent(QGraphicsSceneMouseEvent *event) {
		if (event->button() == Qt::MouseButton::LeftButton) {
			T::setSelected(true);
		}
	}

	QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
					const QVariant &value) override {
		switch (change) {
		case QGraphicsItem::ItemPositionHasChanged:
			node_info["pos"] = std::vector<float>{T::x(), T::y()};
			break;
		case QGraphicsItem::ItemTransformOriginPointHasChanged:
			node_info["origin"] = std::vector<float>{
					T::transformOriginPoint().x(),
					T::transformOriginPoint().y()};
			break;
		case QGraphicsItem::ItemTransformHasChanged:
		{
			QPointF val{1, 1};
			val = val * T::transform();
			node_info["scale"] = std::vector<float>{
					static_cast<float>(val.x()),
					static_cast<float>(val.y())};
			break;
		}
		case QGraphicsItem::ItemRotationHasChanged:
			node_info["rotation"] = T::rotation();
			break;
		case QGraphicsItem::ItemSelectedHasChanged:
			if (T::isSelected())
				show_controls();
			else
				hide_controls();
			break;
		default:
			break;
		}
		return value;
	}

	void show_controls() override {
		or_con->show();
		sc_con[0]->setPos(T::boundingRect().width()/2,
				T::boundingRect().height());
		sc_con[1]->setPos(T::boundingRect().width(),
				T::boundingRect().height());
		sc_con[2]->setPos(T::boundingRect().width(),
				T::boundingRect().height()/2);
		for (size_t i = 0; i < 3; i++)
			sc_con[i]->show();
	}

	void hide_controls() override {
		or_con->hide();
		for (size_t i = 0; i < 3; i++)
			sc_con[i]->hide();
	}

private:
	OriginControl *or_con = nullptr;
	ScaleControl *sc_con[3] = {nullptr, nullptr, nullptr};
};

#endif // NODE_H
