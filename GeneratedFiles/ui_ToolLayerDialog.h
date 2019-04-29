/********************************************************************************
** Form generated from reading UI file 'ToolLayerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLLAYERDIALOG_H
#define UI_TOOLLAYERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ToolLayerDialog
{
public:
    QPushButton *pushButton_quit;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QPushButton *pushButton_ok;
    QLabel *label;

    void setupUi(QDialog *ToolLayerDialog)
    {
        if (ToolLayerDialog->objectName().isEmpty())
            ToolLayerDialog->setObjectName(QString::fromUtf8("ToolLayerDialog"));
        ToolLayerDialog->resize(295, 157);
        pushButton_quit = new QPushButton(ToolLayerDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(180, 100, 75, 23));
        lineEdit_2 = new QLineEdit(ToolLayerDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(140, 40, 71, 20));
        label_2 = new QLabel(ToolLayerDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 40, 71, 16));
        pushButton_ok = new QPushButton(ToolLayerDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(60, 100, 75, 23));
        label = new QLabel(ToolLayerDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(220, 40, 51, 20));

        retranslateUi(ToolLayerDialog);
        QObject::connect(pushButton_quit, SIGNAL(clicked()), ToolLayerDialog, SLOT(close()));
        QObject::connect(pushButton_ok, SIGNAL(clicked(bool)), ToolLayerDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(ToolLayerDialog);
    } // setupUi

    void retranslateUi(QDialog *ToolLayerDialog)
    {
        ToolLayerDialog->setWindowTitle(QApplication::translate("ToolLayerDialog", "\345\210\200\345\205\267\346\233\262\351\235\242\345\210\206\345\261\202\345\217\202\346\225\260", nullptr));
        pushButton_quit->setText(QApplication::translate("ToolLayerDialog", "\345\217\226\346\266\210", nullptr));
        lineEdit_2->setText(QApplication::translate("ToolLayerDialog", "2.0", nullptr));
        label_2->setText(QApplication::translate("ToolLayerDialog", "\345\261\202\351\253\230\357\274\232", nullptr));
        pushButton_ok->setText(QApplication::translate("ToolLayerDialog", "\347\241\256\345\256\232", nullptr));
        label->setText(QApplication::translate("ToolLayerDialog", "mm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToolLayerDialog: public Ui_ToolLayerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLLAYERDIALOG_H
