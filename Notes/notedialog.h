#ifndef NOTEDIALOG_H
#define NOTEDIALOG_H

#include "noteclass.h"
#include <QDialog>

namespace Ui {
    class NoteDialog;
}

class NoteDialog : public QDialog {
    Q_OBJECT

public:
    explicit NoteDialog(QWidget *parent = nullptr);
    ~NoteDialog();

    // Use it to initialize Dialog
    void initDialog(NoteClass *note, QStringList tags);

    // Return new [edired] note
    NoteClass* getNewNote();

private slots:
    void on_buttonAddTag_clicked();
    void on_buttonDeleteNote_clicked();
    void on_buttonRemoveTag_clicked();
    void on_buttonSave_clicked();

private:
    Ui::NoteDialog *ui;

    NoteClass *note;
    // All tags, not tags from current Note
    QStringList tags;

    // Function to display stuff
    void update();

};

#endif // NOTEDIALOG_H
