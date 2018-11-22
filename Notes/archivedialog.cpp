#include "archivedialog.h"
#include "ui_archivedialog.h"

#include "noteclass.h"
#include "singlenoteview.h"
#include "additionalclass.h"

#include <QMenu>

ArchiveDialog::ArchiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchiveDialog) {
    ui->setupUi(this);

    ui->listNotes->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listNotes, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showNotesMenu(QPoint)));
}

ArchiveDialog::~ArchiveDialog() {
    delete ui;
}

void ArchiveDialog::setNotes(QVector<NoteClass *> notes) {
    this->notes = notes;
    update();
}

QVector<int> ArchiveDialog::getToUnarchive() {
    return this->toUnarchive;
}

QVector<int> ArchiveDialog::getToDelete() {
    return this->toDelete;
}

void ArchiveDialog::showNotesMenu(const QPoint &pos) {
    QPoint globalPos = ui->listNotes->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Move from archive", this, SLOT(unarchiveNote()));
    myMenu.addAction("Delete Note",  this, SLOT(removeNote()));
    myMenu.exec(globalPos);
}

void ArchiveDialog::removeNote() {
    int listSize = ui->listNotes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {
        if (AdditionalClass::checkIfOk("Do you want to delete note?", "Delete note")) {
            int deleteID = findNoteID(z);
            this->toDelete.push_back(deleteID);
            int notesSize = notes.size();

            for (int i = 0; i < notesSize; i++)
                if (notes[i]->getID() == deleteID) {
                    notes.erase(notes.begin() + i);
                    break;
                }
            update();
        }
    }
}

void ArchiveDialog::unarchiveNote() {
    int listSize = ui->listNotes->selectedItems().size();
        for (int z = 0; z < listSize; z++) {
            if (AdditionalClass::checkIfOk("Do you want to move Note from archive?", "Move from archive")) {

                int deleteID = findNoteID(z);
                this->toUnarchive.push_back(deleteID);
                int notesSize = notes.size();

                for (int i = 0; i < notesSize; i++)
                    if (notes[i]->getID() == deleteID) {
                        notes.erase(notes.begin() + i);
                        break;
                    }
                update();
            }
        }
}

void ArchiveDialog::update() {
    int notesSize = this->notes.size();
    ui->listNotes->clear();
    // Display all notes
    for (int i = 0; i < notesSize; i++) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listNotes);
        ui->listNotes->addItem(listWidgetItem);
        SingleNoteView *singleNoteView = new SingleNoteView();
        singleNoteView->setStyleSheet("SingleNoteView {border-bottom: 1px solid #212121}");
        singleNoteView->setNote(this->notes[i]);
        listWidgetItem->setSizeHint(QSize(singleNoteView->sizeHint().width(), 75));
        ui->listNotes->setItemWidget(listWidgetItem, singleNoteView);
    }
}

int ArchiveDialog::findNoteID(int number) {
    int foundID = -1;
    int listWidgetSize = ui->listNotes->count();
    for (int i = 0; i < listWidgetSize; i++)
        if (ui->listNotes->item(i) == ui->listNotes->selectedItems()[number]) {
            foundID = (qobject_cast<SingleNoteView*>(ui->listNotes->itemWidget
                (ui->listNotes->item(ui->listNotes->currentRow()))))->getID();
        }
    return foundID;
}
