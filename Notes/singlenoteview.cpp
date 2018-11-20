#include "singlenoteview.h"
#include "ui_singlenoteview.h"

SingleNoteView::SingleNoteView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleNoteView) {
    ui->setupUi(this);
}

SingleNoteView::~SingleNoteView() {
    delete ui;
}

void SingleNoteView::setNote(NoteClass *note) {

}

void SingleNoteView::update() {

}

int SingleNoteView::getID() {

}

void SingleNoteView::paintEvent(QPaintEvent *) {

}
