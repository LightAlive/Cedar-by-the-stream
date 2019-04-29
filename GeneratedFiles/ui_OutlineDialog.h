/********************************************************************************
** Form generated from reading UI file 'OutlineDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTLINEDIALOG_H
#define UI_OUTLINEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_OutlineDialog
{
public:
    QLineEdit *lineEdit;
    QPushButton *pushButton_quit;
    QPushButton *pushButton_ok;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *OutlineDialog)
    {
        if (OutlineDialog->objectName().isEmpty())
            OutlineDialog->setObjectName(QString::fromUtf8("OutlineDialog"));
        OutlineDialog->resize(271, 163);
        lineEdit = new QLineEdit(OutlineDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(120, 40, 61, 20));
        pushButton_quit = new QPushButton(OutlineDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(160, 100, 75, 23));
        pushButton_ok = new QPushButton(OutlineDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(40, 100, 75, 23));
        label = new QLabel(OutlineDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 71, 16));
        label_2 = new QLabel(OutlineDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(190, 40, 51, 20));

        retranslateUi(OutlineDialog);

        QMetaObject::connectSlotsByName(OutlineDialog);
    } // setupUi

    void retranslateUi(QDialog *OutlineDialog)
    {
        OutlineDialog->setWindowTitle(QApplication::translate("OutlineDialog", "OutlineDialog", nullptr));
        lineEdit->setText(QApplication::translate("OutlineDialog", "2.0", nullptr));
        pushButton_quit->setText(QApplication::translate("OutlineDialog", "\345\217\226\346\266\210", nullptr));
        pushButton_ok->setText(QApplication::translate("OutlineDialog", "\347\241\256\345\256\232", nullptr));
        label->setText(QApplication::translate("OutlineDialog", "\345\241\253\345\205\205\351\227\264\350\267\235\357\274\232", nullptr));
        label_2->setText(QApplication::translate("OutlineDialog", "mm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OutlineDialog: public Ui_OutlineDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTLINEDIALOG_H
