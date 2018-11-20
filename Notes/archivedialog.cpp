#include "archivedialog.h"
#include "ui_archivedialog.h"

#include "noteclass.h"
#include "singlenoteview.h"

ArchiveDialog::ArchiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchiveDialog) {
    ui->setupUi(this);
}

ArchiveDialog::~ArchiveDialog() {
    delete ui;
}

void ArchiveDialog::setNotes(QVector<NoteClass *> notes) {
    this->notes = notes;
}

QVector<int> ArchiveDialog::getToUnarchive() {
    return this->toUnarchive;
}

QVector<int> ArchiveDialog::getToDelete() {
    return this->toDelete;
}

void ArchiveDialog::update() {
    int notesSize = this->notes.size();
    ui->listNotes->clear();
    // Display all notes
    for (int i = 0; i < notesSize; i++) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listNotes);
        ui->listNotes->addItem(listWidgetItem);
        SingleNoteView *singleNoteView = new SingleNoteView();
        singleNoteView->setStyleSheet("singleNoteView {border-bottom: 1px solid #212121}");
        singleNoteView->setNote(this->notes[i]);
        listWidgetItem->setSizeHint(QSize(singleNoteView->sizeHint().width(), 85));
        ui->listNotes->setItemWidget(listWidgetItem, singleNoteView);
    }
}
