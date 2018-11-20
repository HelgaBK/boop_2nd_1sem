#ifndef ADDITIONALCLASS_H
#define ADDITIONALCLASS_H

#include <QString>
#include <QWidget>

class AdditionalClass {
public:
    AdditionalClass();
    static void errorMessage(QString message);
    static bool checkIfOk(QString message, QString title);
};

#endif // ADDITIONALCLASS_H
