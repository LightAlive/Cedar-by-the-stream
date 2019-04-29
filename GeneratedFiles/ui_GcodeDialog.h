/********************************************************************************
** Form generated from reading UI file 'GcodeDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GCODEDIALOG_H
#define UI_GCODEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GcodeDialog
{
public:
    QLineEdit *lineEdit;
    QPushButton *pushButton_quit;
    QLabel *label_2;
    QPushButton *pushButton_ok;
    QLabel *label_3;

    void setupUi(QDialog *GcodeDialog)
    {
        if (GcodeDialog->objectName().isEmpty())
            GcodeDialog->setObjectName(QString::fromUtf8("GcodeDialog"));
        GcodeDialog->resize(310, 173);
        lineEdit = new QLineEdit(GcodeDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(150, 50, 51, 20));
        pushButton_quit = new QPushButton(GcodeDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(190, 110, 75, 23));
        label_2 = new QLabel(GcodeDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 50, 71, 16));
        pushButton_ok = new QPushButton(GcodeDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(70, 110, 75, 23));
        label_3 = new QLabel(GcodeDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(210, 50, 51, 20));
        pushButton_ok->raise();
        lineEdit->raise();
        pushButton_quit->raise();
        label_2->raise();
        label_3->raise();

        retranslateUi(GcodeDialog);
        QObject::connect(pushButton_ok, SIGNAL(clicked(bool)), GcodeDialog, SLOT(close()));
        QObject::connect(pushButton_quit, SIGNAL(clicked()), GcodeDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(GcodeDialog);
    } // setupUi

    void retranslateUi(QDialog *GcodeDialog)
    {
        GcodeDialog->setWindowTitle(QApplication::translate("GcodeDialog", "GcodeDialog", nullptr));
        pushButton_quit->setText(QApplication::translate("GcodeDialog", "\345\217\226\346\266\210", nullptr));
        label_2->setText(QApplication::translate("GcodeDialog", "\346\211\223\345\215\260\351\200\237\345\272\246\357\274\232", nullptr));
        pushButton_ok->setText(QApplication::translate("GcodeDialog", "\347\241\256\345\256\232", nullptr));
        label_3->setText(QApplication::translate("GcodeDialog", "mm/s", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GcodeDialog: public Ui_GcodeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GCODEDIALOG_H
