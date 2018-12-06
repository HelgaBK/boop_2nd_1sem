#include "pdfexport.h"
#include "ui_pdfexport.h"

#include <QFileDialog>
#include <QProcess>
#include "additionalclass.h"

PDFexport::PDFexport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PDFexport)
{
    ui->setupUi(this);
}

PDFexport::~PDFexport()
{
    delete ui;
}

void PDFexport::on_cancel_clicked()
{
    this->close();
}

void PDFexport::on_Okey_clicked()
{
    QString password_for_admin, password_for_user, file, path;
        password_for_admin = ui->password_for_admin->text();
        password_for_user = ui->password_for_user->text();
        file = ui->file_name->text();
        path = ui->path_to_file->text();
        if (file == "" || path == "") {
            AdditionalClass::errorMessage("Fill path and file name!");
            ui->path_to_file->setText("");
            ui->file_name->setText("");
            return;
        }
        if ((((password_for_user == password_for_admin) && password_for_user != "") ||
                (password_for_user == "" && password_for_admin != "") || (password_for_user != "" && password_for_admin == ""))
                && (check(password_for_user) && check(password_for_admin))) {
            AdditionalClass::errorMessage("Both passwords must be different & not empty!");
            ui->password_for_admin->setText("");
            ui->password_for_user->setText("");
            return;
        }

        QString fullPath = path;
        fullPath += file + ".pdf";

        generatePDF(fullPath, password_for_admin, password_for_user);
}

void PDFexport::on_three_points_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        "/home",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    ui->path_to_file->setText(dir);
}

bool PDFexport::check(QString password)
{
    for (int i = 0; i < password.size(); i++)
           if (!(
               ((password[i] >= 'a') && (password[i] <= 'z')) ||
               ((password[i] >= 'A') && (password[i] <= 'Z')) ||
               ((password[i] >= '0') && (password[i] <= '9')) ||
               (password[i] == '!') ||
               (password[i] == '@') ||
               (password[i] == '#') ||
               (password[i] == '$') ||
               (password[i] == '%') ||
               (password[i] == '&') ||
               (password[i] == '*') ||
               (password[i] == '(') ||
               (password[i] == ')') ))
               return false;
    return true;
}

void PDFexport::generatePDF(QString &full, QString &password_for_admin, QString &password_for_user)
{
    if (password_for_admin != "" || password_for_user != "") {
            QString command = "included/PDFcreator.exe " + full + " " + password_for_admin + " " + password_for_user;
            QProcess::execute(command);
        }
        else {
            QString command = "included/PDFcreator.exe " + full;
            QProcess::execute(command);
        }
        this->close();
}
