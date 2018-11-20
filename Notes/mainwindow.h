#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "noteclass.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Buttons
    void on_buttonNewNote_clicked();
    void on_buttonNewTag_clicked();

    // Actions from mainToolBar
    void on_actionNewNote_triggered();
    void on_actionNewTag_triggered();
    void on_actionOpenArchive_triggered();

private:
    Ui::MainWindow *ui;

    // Stored values
    QVector<NoteClass*> notes;
    QVector<NoteClass*> archive;

    // Other methods
    void addNote();
    void addTag();
};

#endif // MAINWINDOW_H
