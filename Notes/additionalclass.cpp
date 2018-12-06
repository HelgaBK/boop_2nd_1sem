#include "additionalclass.h"

#include <QErrorMessage>
#include <QMessageBox>

AdditionalClass::AdditionalClass() {}

void AdditionalClass::errorMessage(QString message) {
    QMessageBox messageBox;
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.exec();
}

bool AdditionalClass::checkIfOk(QString message, QString title) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Yes)
        return true;
    else
        return false;
}

