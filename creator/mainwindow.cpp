#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
		QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	setup_scene();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::setup_scene() {
	QSizePolicy sp;
	sp.setHorizontalStretch(5);
	ui->h_splitter->addWidget(&scene_view);
	scene_view.setSizePolicy(sp);
	scene_view.setScene(&scene);
	scene_view.setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::on_actionNew_triggered() {
	scene.addItem(new SpriteNode("Enterprise.png"));
}

void MainWindow::on_actionDelete_triggered() {

}
