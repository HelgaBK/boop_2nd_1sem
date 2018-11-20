#include "mainwindow.h"
#include "singlenoteview.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    if (!readJSON(JSON_SAVE)) {
        this->MaxID = 0;
    }
    // Right clicks for both lists

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

    updateView();
}

MainWindow::~MainWindow() {
    writeJSON(JSON_SAVE);
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

void MainWindow::showTagsMenu(const QPoint &pos) {
    QPoint globalPos = ui->listTags->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("New tag", this, SLOT(addTagItem()));
    myMenu.addAction("Edit tag", this, SLOT(editTagItem()));
    if (!this->tagsFilter.contains(ui->listTags->selectedItems()[0]->text()))
        myMenu.addAction("Add to filter", this, SLOT(addTagToFilter()));
    else
        myMenu.addAction("Remove from filter", this, SLOT(removeTagFromFilter()));
    myMenu.addAction("Delete tag",  this, SLOT(deleteTagItem()));
    myMenu.exec(globalPos);
}

void MainWindow::showNotesMenu(const QPoint &pos) {
    QPoint globalPos = ui->listNotes->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Edit Note", this, SLOT(editNote()));
    myMenu.addAction("Move to archive", this, SLOT(moveToArchive()));
    myMenu.addAction("Delete Note",  this, SLOT(deleteNote()));
    myMenu.exec(globalPos);
}

void MainWindow::addTag() {
    // TODO :: Write function
}

void MainWindow::editTag() {
    // TODO :: Write function
}

void MainWindow::removeTag() {
    // TODO :: Write function
}

void MainWindow::addNote() {
    // TODO :: Write function
}

void MainWindow::editNote() {
    // TODO :: Write function
}

void MainWindow::removeNote() {
    // TODO :: Write function
}

void MainWindow::archiveNote() {
    // TODO :: Write function
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
        singleNoteView->setStyleSheet("singleNoteView {border-bottom: 1px solid #BDBDBD}");
        singleNoteView->setNote(this->notes[i]);
        listWidgetItem->setSizeHint(QSize(singleNoteView->sizeHint().width(), 85));
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
        if (this->tags[i] != "work" && this->tags[i] != "university" && this->tags[i] != "uncategorized")
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
