#include "notedialog.h"
#include "ui_notedialog.h"

NoteDialog::NoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoteDialog)
{
    ui->setupUi(this);
}

NoteDialog::~NoteDialog()
{
    delete ui;
}
