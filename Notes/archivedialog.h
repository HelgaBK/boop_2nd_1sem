#ifndef ARCHIVEDIALOG_H
#define ARCHIVEDIALOG_H

#include "noteclass.h"

#include <QDialog>

namespace Ui {
class ArchiveDialog;
}

class ArchiveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArchiveDialog(QWidget *parent = nullptr);
    ~ArchiveDialog();

    void setNotes(QVector<NoteClass*> notes);
    QVector<int> getToUnarchive();
    QVector<int> getToDelete();

public slots:
    // Right-click actions
    void showListMenu(const QPoint &pos);
    void removeNote();
    void unarchiveNote();

private:
    Ui::ArchiveDialog *ui;

    // Stored values
    QVector<NoteClass*> notes;
    QVector<int> toUnarchive;
    QVector<int> toDelete;

    // Interface methods
    void update();

    // Additional method
    int findNoteID(int number);
};

#endif // ARCHIVEDIALOG_H
