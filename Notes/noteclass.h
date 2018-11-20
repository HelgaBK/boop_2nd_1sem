#ifndef NOTECLASS_H
#define NOTECLASS_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>



class NoteClass {
public:
    NoteClass();
    NoteClass(int ID, QString text, QDateTime editedTime, QStringList tags);
    NoteClass(int ID, QString text, QDateTime editedTime);
    NoteClass(const QJsonObject &json);

    // Funcitons to work with JSON
    void readJSON(const QJsonObject &json);
    void writeJSON(QJsonObject &json) const;

    int getID();

    QString getText() const;
    void setText(QString text);

    QDateTime getEditedTime() const;
    void setEditedTime(QDateTime editedTime);

    void setTags(QStringList tags);
    QStringList getTags() const;
    bool contains(QString tag) const;
    bool addTag(QString tag);
    bool removeTag(QString tag);
    void clearTags();

    QString getTagsInString() const;

private:
    int ID;
    QString text;
    QDateTime editedTime;
    QStringList tags;
};

#endif // NOTECLASS_H
