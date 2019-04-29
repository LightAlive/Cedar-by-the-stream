/********************************************************************************
** Form generated from reading UI file 'GrowthLineLayerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROWTHLINELAYERDIALOG_H
#define UI_GROWTHLINELAYERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GrowthLineLayerDialog
{
public:
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_ok;
    QLabel *label;
    QPushButton *pushButton_quit;
    QLabel *label_2;

    void setupUi(QDialog *GrowthLineLayerDialog)
    {
        if (GrowthLineLayerDialog->objectName().isEmpty())
            GrowthLineLayerDialog->setObjectName(QString::fromUtf8("GrowthLineLayerDialog"));
        GrowthLineLayerDialog->resize(325, 126);
        lineEdit_2 = new QLineEdit(GrowthLineLayerDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(140, 40, 71, 20));
        pushButton_ok = new QPushButton(GrowthLineLayerDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(60, 90, 75, 23));
        label = new QLabel(GrowthLineLayerDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(220, 40, 51, 20));
        pushButton_quit = new QPushButton(GrowthLineLayerDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(180, 90, 75, 23));
        label_2 = new QLabel(GrowthLineLayerDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 40, 71, 16));

        retranslateUi(GrowthLineLayerDialog);
        QObject::connect(pushButton_ok, SIGNAL(clicked(bool)), GrowthLineLayerDialog, SLOT(close()));
        QObject::connect(pushButton_quit, SIGNAL(clicked(bool)), GrowthLineLayerDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(GrowthLineLayerDialog);
    } // setupUi

    void retranslateUi(QDialog *GrowthLineLayerDialog)
    {
        GrowthLineLayerDialog->setWindowTitle(QApplication::translate("GrowthLineLayerDialog", "\347\224\237\351\225\277\347\272\277\347\255\211\345\261\202\345\216\232\346\233\262\351\235\242\345\210\206\345\261\202\345\217\202\346\225\260", nullptr));
        lineEdit_2->setText(QApplication::translate("GrowthLineLayerDialog", "2.0", nullptr));
        pushButton_ok->setText(QApplication::translate("GrowthLineLayerDialog", "\347\241\256\345\256\232", nullptr));
        label->setText(QApplication::translate("GrowthLineLayerDialog", "mm", nullptr));
        pushButton_quit->setText(QApplication::translate("GrowthLineLayerDialog", "\345\217\226\346\266\210", nullptr));
        label_2->setText(QApplication::translate("GrowthLineLayerDialog", "\345\261\202\351\253\230\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GrowthLineLayerDialog: public Ui_GrowthLineLayerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROWTHLINELAYERDIALOG_H
