#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_buttonNewNote_clicked() {

}

void MainWindow::on_buttonNewTag_clicked() {

}

void MainWindow::on_actionNewNote_triggered() {

}

void MainWindow::on_actionNewTag_triggered() {

}

void MainWindow::on_actionOpenArchive_triggered() {

}
