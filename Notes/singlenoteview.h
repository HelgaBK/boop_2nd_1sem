#ifndef SINGLENOTEVIEW_H
#define SINGLENOTEVIEW_H

#include "noteclass.h"
#include <QWidget>

namespace Ui {
    class SingleNoteView;
}

class SingleNoteView : public QWidget {
    Q_OBJECT

public:
    explicit SingleNoteView(QWidget *parent = nullptr);
    ~SingleNoteView();

    void setNote(NoteClass *note);
    void update();
    int getID();

private:
    Ui::SingleNoteView *ui;
    // To draw bottom line using css
    void paintEvent(QPaintEvent *);

    // Data :
    NoteClass *note;
};

#endif // SINGLENOTEVIEW_H
