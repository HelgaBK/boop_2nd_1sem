#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // TODO :: Add reading from JSON
    // TODO :: Add right clicks for both lists
    // TODO :: Add double-click actions for both lists

    ui->listTags->addItem("uncategorized");
    ui->listTags->addItem("university");
    ui->listTags->addItem("personal");
    this->tags.push_back("university");
    this->tags.push_back("personal");

    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    updateView();
}

MainWindow::~MainWindow() {
    // TODO :: Add writing to JSON
    delete ui;
}

void MainWindow::on_buttonNewNote_clicked() {
    // TODO :: Write function
}

void MainWindow::on_buttonNewTag_clicked() {
    // TODO :: Write function
}

void MainWindow::on_actionNewNote_triggered() {
    // TODO :: Write function
}

void MainWindow::on_actionNewTag_triggered() {
    // TODO :: Write function
}

void MainWindow::on_actionOpenArchive_triggered() {
    // TODO :: Write function
}

void MainWindow::addNote() {
    // TODO :: Write function
}

void MainWindow::addTag() {
    // TODO :: Write function
}

void MainWindow::updateView() {
    // TODO :: Write function
}

void MainWindow::updateList() {
    // TODO :: Write function
}

void MainWindow::readJSON(QString file) {
    // TODO :: Write function
}

void MainWindow::writeJSON(QString file) {
    // TODO :: Write function
}
