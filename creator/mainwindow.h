#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "world_view.h"
#include "nodes/node_2d.h"
#include "nodes/sprite_node.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_actionNew_triggered();
	void on_actionDelete_triggered();

private:
	json scene_info;
	QGraphicsScene scene;
	WorldView scene_view;
	QList<AbstractNode *> selected_nodes;
	Ui::MainWindow *ui;

	void setup_scene();
};
#endif // MAINWINDOW_H
