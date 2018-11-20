#include "noteclass.h"

#include <QJsonArray>

NoteClass::NoteClass() {

}

NoteClass::NoteClass(int ID, QString text, QDateTime editedTime, QStringList tags) {
    this->ID = ID;
    this->text = text;
    this->editedTime = editedTime;
    this->tags = tags;
}

NoteClass::NoteClass(int ID, QString text, QDateTime editedTime) {
    this->ID = ID;
    this->text = text;
    this->editedTime = editedTime;
    this->tags.push_back("uncategorized");
}

NoteClass::NoteClass(const QJsonObject &json) {
    readJSON(json);
}

void NoteClass::readJSON(const QJsonObject &json) {
    if (json.contains("id") && json["id"].isDouble())
        this->ID = json["id"].toInt();

    if (json.contains("time") && json["time"].isString())
        this->editedTime = QDateTime().fromString(json["time"].toString(), Qt::TextDate);

    if (json.contains("text") && json["text"].isString())
        this->text = json["text"].toString();

    if (json.contains("tags") && json["tags"].isArray()) {
        QJsonArray tagsArray = json["tags"].toArray();
        int numberOfTags = tagsArray.count();
        for (int i = 0; i < numberOfTags; i++)
            this->tags.push_back(tagsArray[i].toString());
    }
}

void NoteClass::writeJSON(QJsonObject &json) const {
    json["id"] = this->ID;
        json["time"] = this->editedTime.toString(Qt::TextDate);
        json["text"] = this->text;
        QJsonArray tagsArray;
        int tagsSize = this->tags.size();
        for (int i = 0; i < tagsSize; i++)
            tagsArray.push_back(this->tags[i]);
        json["tags"] = tagsArray;
}

int NoteClass::getID() {
    return this->ID;
}

QString NoteClass::getText() const {
    return this->text;
}

void NoteClass::setText(QString text) {
    this->text = text;
}

QDateTime NoteClass::getEditedTime() const {
    return this->editedTime;
}

void NoteClass::setEditedTime(QDateTime editedTime) {
    this->editedTime = editedTime;
}

void NoteClass::setTags(QStringList tags) {
    this->tags = tags;
}

QStringList NoteClass::getTags() const {
    return this->tags;
}

bool NoteClass::contains(QString tag) const {
    return this->tags.contains(tag);
}

bool NoteClass::addTag(QString tag) {
    if (this->tags.size() == 1 && this->tags[0] == "uncategorized") {
        this->tags.clear();
        this->tags.push_back(tag);
        return true;
    }
    if (!(this->tags.contains(tag))) {
        this->tags.push_back(tag);
        return true;
    }
    return false;
}

bool NoteClass::removeTag(QString tag) {
    if (this->tags.contains(tag)) {
        for (int i = 0; i < this->tags.size(); i++)
            if (this->tags[i] == tag) {
                this->tags.erase(this->tags.begin() + i);

                // If all tags were deleted
                if (tags.size() == 0)
                    tags.push_back("uncategorized");
                return true;
            }
    }
    return false;
}

void NoteClass::clearTags() {
    this->tags.clear();
}

QString NoteClass::getTagsInString() const {
    QString tagsString = "";
    int tagsSize = this->tags.size();
    for (int i = 0; i < tagsSize - 1; i++)
        tagsString += this->tags[i] + ", ";
    tagsString += this->tags[tagsSize - 1];
    return tagsString;
}
