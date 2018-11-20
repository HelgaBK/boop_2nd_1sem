#include "notedialog.h"
#include "ui_notedialog.h"
#include "additionalclass.h"

#include <QInputDialog>

NoteDialog::NoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoteDialog) {
    ui->setupUi(this);
}

NoteDialog::~NoteDialog() {
    delete ui;
}

void NoteDialog::initDialog(NoteClass *note, QStringList tags) {
    this->note = note;
    this->tags = tags;
}

void NoteDialog::on_buttonAddTag_clicked() {
    bool ok;
    QString tag = QInputDialog::getItem(this, tr("Add tag : "), tr("Tag name:"), this->tags, 0, false, &ok);
    if (ok && this->note->contains(tag))
        AdditionalClass::errorMessage("This note has a " + tag);
    else if (ok) {
        this->note->addTag(tag);
    }
    update();
}

void NoteDialog::on_buttonDeleteNote_clicked() {
    // In mainwindow all empty notes will be deleted
    this->note->setText("");
    this->close();
}

void NoteDialog::on_buttonRemoveTag_clicked() {
    bool ok;
    QString tag = QInputDialog::getItem(this, tr("Add tag : "), tr("Tag name:"), this->tags, 0, false, &ok);
    if (ok && !this->note->contains(tag))
        AdditionalClass::errorMessage("You can't delete " + tag);
    else if (ok) {
        this->note->removeTag(tag);
        if (this->note->getTags().size() == 0)
            this->note->addTag("uncategorized");
    }
    update();
}

void NoteDialog::on_buttonSave_clicked() {
    this->note->setEditedTime(QDateTime::currentDateTime());
    this->note->setText(ui->text->toPlainText());
    this->close();
}

void NoteDialog::update() {
    QStringList currentTags = this->note->getTags();
    int currentTagsSize = currentTags.size();
    if (currentTagsSize == 0) {
        this->note->addTag("uncategorized");
        ui->tags->setText("uncategorized");
    }
    else {
        QString tagsString = "";
        for (int i = 0; i < currentTagsSize - 1; i++) {
            tagsString += currentTags[i] + ", ";
        }
        tagsString += currentTags[currentTagsSize - 1];
        ui->tags->setText(tagsString);
    }
}
