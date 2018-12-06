#include "additionalclass.h"
#include "archivedialog.h"
#include "mainwindow.h"
#include "notedialog.h"
#include "singlenoteview.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Right clicks for both lists!
    ui->listTags->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->listTags, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTagsMenu(QPoint)));
    ui->listNotes->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listNotes, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showNotesMenu(QPoint)));
    // Double-click actions for both lists
    connect(ui->listTags, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editTag()));
    connect(ui->listNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editNote()));

    // Add standard tags [can't be removed]
    ui->listTags->addItem("uncategorized");
    ui->listTags->addItem("university");
    ui->listTags->addItem("personal");
    this->tags.push_back("university");
    this->tags.push_back("personal");

    // Disable right-click for MainToolBar
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    if (!readJSON(JSON_SAVE)) {
        this->MaxID = 0;
    }

    updateView();
}

MainWindow::~MainWindow() {
    writeJSON(JSON_SAVE);
    delete ui;
}

void MainWindow::on_buttonNewNote_clicked() {
    addNote();
}

void MainWindow::on_buttonNewTag_clicked() {
    addTag();
}

void MainWindow::on_actionNewNote_triggered() {
    addNote();
}

void MainWindow::on_actionNewTag_triggered() {
    addTag();
}

void MainWindow::on_actionOpenArchive_triggered() {
    ArchiveDialog *archiveDialog = new ArchiveDialog();
    archiveDialog->setNotes(this->archive);
    archiveDialog->exec();

    QVector<int> deletedNotes = archiveDialog->getToDelete();
    QVector<int> unarchivedNotes = archiveDialog->getToUnarchive();

    if (deletedNotes.size() > 0)
        for (int j = 0; j < deletedNotes.size(); j++)
            for (int i = 0; i < archive.size(); i++) {
                if (this->archive[i]->getID() == deletedNotes[j]) {
                    archive.erase(archive.begin() + i);
                    break;
                }
            }
    if (unarchivedNotes.size() > 0)
        for (int j = 0; j < unarchivedNotes.size(); j++)
            for (int i = 0; i < archive.size(); i++) {
                if (this->archive[i]->getID() == unarchivedNotes[j]) {
                    notes.push_back(archive[i]);
                    archive.erase(archive.begin() + i);
                    break;
                }
            }
    updateView();
}

void MainWindow::showTagsMenu(const QPoint &pos) {
    QPoint globalPos = ui->listTags->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("New tag", this, SLOT(addTag()));
    myMenu.addAction("Edit tag", this, SLOT(editTag()));
    if (!this->tagsFilter.contains(ui->listTags->selectedItems()[0]->text()))
        myMenu.addAction("Add to filter", this, SLOT(addTagToFilter()));
    else
        myMenu.addAction("Remove from filter", this, SLOT(removeTagFromFilter()));
    myMenu.addAction("Delete tag",  this, SLOT(removeTag()));
    myMenu.exec(globalPos);
}

void MainWindow::showNotesMenu(const QPoint &pos) {
    QPoint globalPos = ui->listNotes->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Edit Note", this, SLOT(editNote()));
    myMenu.addAction("Move to archive", this, SLOT(archiveNote()));
    myMenu.addAction("Delete Note",  this, SLOT(removeNote()));
    myMenu.exec(globalPos);
}

void MainWindow::addTag() {
    bool ok;
    QString tag = QInputDialog::getText(this, tr("Create New Tag"), tr("Tag name:"), QLineEdit::Normal, "default", &ok);
    tag = tag.toLower();
    if (ok && !tag.isEmpty()) {
        // Check for existing
        int numberOfTags = ui->listTags->count();
        bool hasSame = false;
        for (int i = 0; i < numberOfTags; i++)
            if (ui->listTags->item(i)->text() == tag)
                hasSame = true;
        if (!hasSame) {
            ui->listTags->addItem(tag);
            this->tags.push_back(tag);
        }
        else {
            // If tag is already available
            AdditionalClass::errorMessage("There is already " + tag + " tag");
        }
    }
}

void MainWindow::editTag() {
    // Just accept this function
    int tagsSize = ui->listTags->selectedItems().size();
    for (int k = 0; k < tagsSize; ++k) {
        if (ui->listTags->selectedItems()[k]->text() == "uncategorized"
                || ui->listTags->selectedItems()[k]->text() == "university"
                || ui->listTags->selectedItems()[k]->text() == "personal")
            AdditionalClass::errorMessage("This action is prohibited!");
        else {
            QString tagToEdit = ui->listTags->selectedItems()[k]->text();
            int tagSize = tags.size();
            for (int i = 0; i < tagSize; i++)
                if (tags[i] == tagToEdit) {
                    bool ok;
                    QString tag = QInputDialog::getText(this, tr("Edit Tag"), tr("Tag name:"),
                        QLineEdit::Normal, ui->listTags->selectedItems()[k]->text(), &ok);
                    tag = tag.toLower(); // Get unified look for tags

                    if (ok && !tag.isEmpty()) {
                        // Check for similar tag
                        int numberOfTags = ui->listTags->count();
                        bool hasSame = false;
                        for (int i = 0; i < numberOfTags; i++)
                            if (ui->listTags->item(k)->text() == tag)
                                hasSame = true;
                        if (!hasSame) {

                            // Find selected tag
                            int notesSize = this->notes.size();
                            for (int i = 0; i < notesSize; i++)
                                if (this->notes[i]->contains(tagToEdit)) {
                                    this->notes[i]->removeTag(tagToEdit);
                                    this->notes[i]->addTag(tag);
                                }

                            // Set new tag value
                            for (int j = 0; j < tagSize; j++) {
                                if (this->tags[j] == tagToEdit) {
                                    this->tags[j] = tag;
                                    ui->listTags->selectedItems()[k]->setText(tag);
                                }
                            }
                        }
                        else if (!ok) {
                            // If tag is already available
                            AdditionalClass::errorMessage("There is already " + tag + " tag");
                        }
                    }
                    break;
                }
        }
    }
    updateView();
}

void MainWindow::removeTag() {
    int tagsSize = ui->listTags->selectedItems().size();
    for (int i = 0; i < tagsSize; ++i) {
        if (AdditionalClass::checkIfOk("Do you want to delete this tag?", "Delete tag")) {
            if (ui->listTags->selectedItems()[i]->text() == "uncategorized"
                    || ui->listTags->selectedItems()[i]->text() == "university"
                    || ui->listTags->selectedItems()[i]->text() == "personal") {
                AdditionalClass::errorMessage("You can't remove " + ui->listTags->selectedItems()[i]->text() + " tag");
            }
            else {
                QListWidgetItem *item = ui->listTags->takeItem(ui->listTags->currentRow());
                int tagSize = this->tags.size();
                for (int j = 0; j < tagSize; j++)
                    if (this->tags[j] == item->text()) {
                        this->tags.erase(tags.begin() + j);
                        break;
                    }

                // Delete removed tag from all notes
                int notesSize = notes.size();
                int archiveSize = archive.size();
                for (int j = 0; j < notesSize; j++)
                    this->notes[j]->removeTag(item->text());
                for (int j = 0; j < archiveSize; j++)
                    this->archive[j]->removeTag(item->text());

                delete item;
            }
        }
    }
    updateView();
}

void MainWindow::addTagToFilter() {
    int listSize = ui->listTags->selectedItems().size();
    for (int i = 0; i < listSize; ++i) {
        if (!this->tagsFilter.contains(ui->listTags->selectedItems()[i]->text()))
            this->tagsFilter.push_back(ui->listTags->selectedItems()[i]->text());
        else {
            AdditionalClass::errorMessage("There is already " + ui->listTags->selectedItems()[i]->text() + " tag");
        }
    }
    updateView();
}

void MainWindow::removeTagFromFilter() {
    int listSize = ui->listTags->selectedItems().size();
    for (int i = 0; i < listSize; ++i) {
        if (this->tagsFilter.contains(ui->listTags->selectedItems()[i]->text()))
            for (int j = 0; j < this->tagsFilter.size(); j++)
                if (this->tagsFilter[j] == ui->listTags->selectedItems()[i]->text()) {
                    this->tagsFilter.erase(this->tagsFilter.begin() + j);
                    break;
                }
                else continue;
        else
            AdditionalClass::errorMessage("There is no " + ui->listTags->selectedItems()[i]->text() + " tag");
    }
    updateView();
}

void MainWindow::addNote() {
    QDateTime currentTime = QDateTime::currentDateTime();
    QString text = "";
    QStringList noteTags;
    int ID = this->MaxID + 1;
    this->MaxID++;
    NoteClass *noteClass = new NoteClass(ID, text, currentTime);
    NoteDialog *noteDialog = new NoteDialog(this);
    noteDialog->initDialog(noteClass, this->tags);
    noteDialog->exec();
    if (noteClass->getText() == "") { }
    else this->notes.push_back(noteClass);
    updateView();
}

void MainWindow::editNote() {
    int notesSize = ui->listNotes->selectedItems().size();
    for (int z = 0; z < notesSize; z++) {

        int editID = -1;
        int listWidgetSize = ui->listNotes->count();
        for (int i = 0; i < listWidgetSize; i++)
            if (ui->listNotes->item(i) == ui->listNotes->selectedItems()[z]) {
                editID = (qobject_cast<SingleNoteView*>(ui->listNotes->itemWidget(
                    ui->listNotes->item(ui->listNotes->currentRow()))))->getID();
            }
        if (editID == -1)
            AdditionalClass::errorMessage("That wasn't suppose to happen");

        int notesSize = notes.size();
        NoteClass *noteToEdit = new NoteClass();

        for (int i = 0; i < notesSize; i++)
            if (notes[i]->getID() == editID) {
                noteToEdit = notes[i];
                break;
            }
        QDateTime editedTime = noteToEdit->getEditedTime();
        QString text = noteToEdit->getText();
        QStringList noteTags = noteToEdit->getTags();
        int ID = noteToEdit->getID();

        NoteClass *noteClass = new NoteClass(ID, text, editedTime, noteTags);
        NoteDialog *noteDialog = new NoteDialog(this);
        noteDialog->initDialog(noteClass, this->tags);
        noteDialog->exec();
        noteClass = noteDialog->getNewNote();
        if (noteClass->getText() == "") {
            int notesSize = notes.size();
            for (int i = 0; i < notesSize; i++)
                if (this->notes[i]->getID() == noteClass->getID()) {
                    // Remove item
                    this->notes.erase(this->notes.begin() + i);
                    break;
                }
        }
        else {
            noteToEdit->clearTags();
            noteToEdit->setText(noteClass->getText());
            QStringList tagsListNote = noteClass->getTags();
            for (int i = 0; i < tagsListNote.size(); i++)
                if (!noteToEdit->contains(tagsListNote[i]))
                    noteToEdit->addTag(tagsListNote[i]);
            noteToEdit->setEditedTime(noteClass->getEditedTime());
        }
    }
    updateView();
}

void MainWindow::removeNote() {
    int listSize = ui->listNotes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {
        if (AdditionalClass::checkIfOk("Do you want to delete this note?", "Delete note")) {
            int foundID = -1;
            int listWidgetSize = ui->listNotes->count();
            for (int i = 0; i < listWidgetSize; i++)
                if (ui->listNotes->item(i) == ui->listNotes->selectedItems()[z]) {
                    foundID = (qobject_cast<SingleNoteView*>(ui->listNotes->itemWidget(
                        ui->listNotes->item(ui->listNotes->currentRow()))))->getID();
                }

            int notesSize = notes.size();
            for (int i = 0; i < notesSize; i++)
                if (notes[i]->getID() == foundID) {
                    notes.erase(notes.begin() + i);
                    break;
                }
            updateView();
        }
    }
}

void MainWindow::archiveNote() {
    int listSize = ui->listNotes->selectedItems().size();
    for (int z = 0; z < listSize; z++) {
        if (AdditionalClass::checkIfOk("Do you want to move note to archive?", "Move to archive")) {
            int foundID = -1;
            int listWidgetSize = ui->listNotes->count();
            for (int i = 0; i < listWidgetSize; i++)
                if (ui->listNotes->item(i) == ui->listNotes->selectedItems()[z]) {
                    // Confusing row
                    foundID = (qobject_cast<SingleNoteView*>(ui->listNotes->itemWidget(
                        ui->listNotes->item(ui->listNotes->currentRow()))))->getID();
                }

            int notesSize = notes.size();

            for (int i = 0; i < notesSize; i++)
                if (notes[i]->getID() == foundID) {
                    archive.push_back(notes[i]);
                    notes.erase(notes.begin() + i);
                    break;
                }
            updateView();
        }
    }
}

void MainWindow::updateView() {
    updateList();
    QString labelText = "";
    if (this->tagsFilter.size() != 0) {
        labelText += "Notes [";
        for (int i = 0; i < tagsFilter.size(); i++) {
            labelText += " " + tagsFilter[i];
        }
        labelText += " ]";
    }
    else
        labelText = "Notes";
    ui->labelNotes->setText(labelText);
}

void MainWindow::updateList() {
    int notesSize = this->notes.size();
    int filterSize = this->tagsFilter.size();
    QVector<NoteClass*> selectedNotes;
    if (tagsFilter.size() == 0) {
        selectedNotes = this->notes;
    }
    else {
        // If the tags filter has some selected tags
        bool hasAllTags;
        for (int i = 0; i < notesSize; i++) {
            hasAllTags = true;
            for (int j = 0; j < filterSize; j++)
                if (!this->notes[i]->contains(tagsFilter[j]))
                    hasAllTags = false;
            if (hasAllTags)
                selectedNotes.push_back(notes[i]);
        }
    }

    int noteListSize = selectedNotes.size();
    ui->listNotes->clear();
    for (int i = 0; i < noteListSize; i++) {
        QListWidgetItem *listWidgetItem = new QListWidgetItem(ui->listNotes);
        ui->listNotes->addItem(listWidgetItem);
        SingleNoteView *singleNoteView = new SingleNoteView;
        singleNoteView->setStyleSheet("SingleNoteView {border-bottom: 1px solid #BDBDBD}");
        singleNoteView->setNote(selectedNotes[i]);
        listWidgetItem->setSizeHint(QSize(singleNoteView->sizeHint().width(), 75));
        ui->listNotes->setItemWidget(listWidgetItem, singleNoteView);
    }
}

bool MainWindow::readJSON(QString file) {
    QFile readNotesFile;
    readNotesFile.setFileName(file);
    if (!readNotesFile.open(QFile::ReadOnly | QFile::Text))
        return false;

    // Get JSON data
    QString jsonData = readNotesFile.readAll();
    QJsonDocument document = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObject = document.object();

    // Get array of notes
    if (jsonObject.contains("notes") && jsonObject["notes"].isArray()) {
        QJsonArray jsonArray = jsonObject["notes"].toArray();
        int arraySize = jsonArray.count();
        for (int i = 0; i < arraySize; i++) {
            NoteClass *note = new NoteClass(jsonArray[i].toObject());
            this->notes.push_back(note);
        }
    }

    // Get array of archived notes
    if (jsonObject.contains("archive") && jsonObject["archive"].isArray()) {
        QJsonArray jsonArray = jsonObject["archive"].toArray();
        int arraySize = jsonArray.count();
        for (int i = 0; i < arraySize; i++) {
            NoteClass *note = new NoteClass(jsonArray[i].toObject());
            this->archive.push_back(note);
        }
    }


    // Get max ID
    if (jsonObject.contains("max_id") && jsonObject["max_id"].isDouble())
        this->MaxID = jsonObject["max_id"].toInt();

    // Get list of user-defined tags
    if (jsonObject.contains("tags") && jsonObject["tags"].isArray()) {
        QJsonArray tagArray = jsonObject["tags"].toArray();
        int tagsSize = tagArray.size();
        for (int i = 0; i < tagsSize; i++) {
            this->tags.push_back(tagArray[i].toString());
            ui->listTags->addItem(tagArray[i].toString());
        }
    }

    readNotesFile.close();
    return true;
}

bool MainWindow::writeJSON(QString file) {
    QFile writeNotesFile;
    writeNotesFile.setFileName(file);
    if (!writeNotesFile.open(QFile::WriteOnly))
        return false;

    // Add notes
    QJsonArray notesArray;
    int notesSize = notes.size();
    QJsonObject singleNoteObject;
    for (int i = 0; i < notesSize; i++) {
        notes[i]->writeJSON(singleNoteObject);
        notesArray.push_back(singleNoteObject);
    }

    // Add archived notes
    QJsonArray archiveArray;
    int archiveSize = archive.size();
    QJsonObject archiveObject;
    for (int i = 0; i < archiveSize; i++) {
        archive[i]->writeJSON(archiveObject);
        archiveArray.push_back(archiveObject);
    }

    // Add tags
    QJsonObject finalObject;
    QJsonArray tagArray;
    int tagArraySize = this->tags.size();
    for (int i = 0; i < tagArraySize; i++) {
        // Add only user-defined notes
        if (this->tags[i] != "uncategorized" && this->tags[i] != "university" && this->tags[i] != "personal")
            tagArray.push_back(this->tags[i]);
    }

    finalObject["notes"] = notesArray;
    finalObject["max_id"] = this->MaxID;
    finalObject["tags"] = tagArray;
    finalObject["archive"] = archiveArray;

    // Write JSON and save file
    QJsonDocument document(finalObject);
    writeNotesFile.write(document.toJson());
    writeNotesFile.close();
    return true;
}
