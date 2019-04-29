/********************************************************************************
** Form generated from reading UI file 'ZigzagDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZIGZAGDIALOG_H
#define UI_ZIGZAGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ZigzagDialog
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton_ok;
    QPushButton *pushButton_quit;
    QLabel *label_2;

    void setupUi(QDialog *ZigzagDialog)
    {
        if (ZigzagDialog->objectName().isEmpty())
            ZigzagDialog->setObjectName(QString::fromUtf8("ZigzagDialog"));
        ZigzagDialog->resize(282, 159);
        label = new QLabel(ZigzagDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 71, 16));
        lineEdit = new QLineEdit(ZigzagDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(120, 40, 71, 20));
        pushButton_ok = new QPushButton(ZigzagDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(40, 100, 75, 23));
        pushButton_quit = new QPushButton(ZigzagDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(160, 100, 75, 23));
        label_2 = new QLabel(ZigzagDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(200, 40, 51, 20));

        retranslateUi(ZigzagDialog);
        QObject::connect(pushButton_quit, SIGNAL(clicked(bool)), ZigzagDialog, SLOT(close()));
        QObject::connect(pushButton_ok, SIGNAL(clicked(bool)), ZigzagDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(ZigzagDialog);
    } // setupUi

    void retranslateUi(QDialog *ZigzagDialog)
    {
        ZigzagDialog->setWindowTitle(QApplication::translate("ZigzagDialog", "Z\345\255\227\345\275\242\350\267\257\345\276\204\345\241\253\345\205\205\345\217\202\346\225\260", nullptr));
        label->setText(QApplication::translate("ZigzagDialog", "\345\241\253\345\205\205\351\227\264\350\267\235\357\274\232", nullptr));
        lineEdit->setText(QApplication::translate("ZigzagDialog", "2.0", nullptr));
        pushButton_ok->setText(QApplication::translate("ZigzagDialog", "\347\241\256\345\256\232", nullptr));
        pushButton_quit->setText(QApplication::translate("ZigzagDialog", "\345\217\226\346\266\210", nullptr));
        label_2->setText(QApplication::translate("ZigzagDialog", "mm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ZigzagDialog: public Ui_ZigzagDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZIGZAGDIALOG_H
