#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    if (!readJSON(JSON_SAVE)) {
        this->MaxID = 0;
    }
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
