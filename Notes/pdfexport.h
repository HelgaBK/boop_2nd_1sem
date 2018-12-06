#ifndef PDFEXPORT_H
#define PDFEXPORT_H

#include <QDialog>

namespace Ui {
class PDFexport;
}

class PDFexport : public QDialog
{
    Q_OBJECT

public:
    explicit PDFexport(QWidget *parent = nullptr);
    ~PDFexport();
private slots:
    void on_cancel_clicked();
    void on_Okey_clicked();
    void on_three_points_clicked();

private:
    Ui::PDFexport *ui;
    bool check(QString password);

    void generatePDF(QString &full, QString &password_for_admin, QString &password_for_user);
};

#endif // PDFEXPORT_H
