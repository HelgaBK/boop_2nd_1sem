#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "noteclass.h"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
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

    // Right-click menus
    void showTagsMenu(const QPoint &pos);
    void showNotesMenu(const QPoint &pos);

    // Actions for tags
    void addTag();
    void editTag();
    void removeTag();

    // Actions for notes
    void addNote();
    void editNote();
    void removeNote();
    void archiveNote();

private:
    Ui::MainWindow *ui;

    // Stored values
    QVector<NoteClass*> notes;
    QVector<NoteClass*> archive;
    QStringList tags;
    QStringList tagsFilter;
    int MaxID;

    // Methods to work with interface
    void updateView();
    void updateList();

    // Methods to work with JSON
    bool readJSON(QString file);
    bool writeJSON(QString file);

    // Name of JSON save file
    QString JSON_SAVE = "data.json";
};

#endif // MAINWINDOW_H
