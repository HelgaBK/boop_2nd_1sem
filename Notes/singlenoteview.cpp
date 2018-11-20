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
    // Parsing text
    QString text = note->getText();
    for (int i = 0; i < text.size(); i++)
        if (text[i] == "\n") {
            text = text.left(i);
            break;
        }
    QFontMetrics metrics(ui->text->font());
    QString shortenedText = metrics.elidedText(text, Qt::ElideRight, ui->text->width());
    ui->text->setText(shortenedText);

    ui->editedTime->setText(note->getEditedTime().toString(Qt::TextDate));
    ui->tags->setText(note->getTagsInString());
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
