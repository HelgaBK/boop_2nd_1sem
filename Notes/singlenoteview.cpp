#include "singlenoteview.h"
#include "ui_singlenoteview.h"

#include <QPainter>
#include <QStyleOption>

SingleNoteView::SingleNoteView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleNoteView) {
    ui->setupUi(this);
}

SingleNoteView::~SingleNoteView() {
    delete ui;
}

void SingleNoteView::setNote(NoteClass *note) {
    this->note = note;
    update();
}

void SingleNoteView::update() {
    // TODO :: Add parsing text
    ui->text->setText(note->getText());
    ui->editedTime->setText(note->getEditedTime().toString(Qt::TextDate));
}

int SingleNoteView::getID() {
    return this->note->getID();
}

void SingleNoteView::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter *p = new QPainter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, p, this);
}
