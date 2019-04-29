/********************************************************************************
** Form generated from reading UI file 'GrowthLineVariousLayerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROWTHLINEVARIOUSLAYERDIALOG_H
#define UI_GROWTHLINEVARIOUSLAYERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GrowthLineVariousLayerDialog
{
public:
    QPushButton *pushButton_ok;
    QLabel *label;
    QPushButton *pushButton_quit;
    QLineEdit *lineEdit_1;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QDialog *GrowthLineVariousLayerDialog)
    {
        if (GrowthLineVariousLayerDialog->objectName().isEmpty())
            GrowthLineVariousLayerDialog->setObjectName(QString::fromUtf8("GrowthLineVariousLayerDialog"));
        GrowthLineVariousLayerDialog->resize(328, 163);
        pushButton_ok = new QPushButton(GrowthLineVariousLayerDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(70, 120, 75, 23));
        label = new QLabel(GrowthLineVariousLayerDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(230, 30, 51, 20));
        pushButton_quit = new QPushButton(GrowthLineVariousLayerDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(190, 120, 75, 23));
        lineEdit_1 = new QLineEdit(GrowthLineVariousLayerDialog);
        lineEdit_1->setObjectName(QString::fromUtf8("lineEdit_1"));
        lineEdit_1->setGeometry(QRect(150, 30, 71, 20));
        label_2 = new QLabel(GrowthLineVariousLayerDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 30, 71, 16));
        lineEdit_2 = new QLineEdit(GrowthLineVariousLayerDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(150, 70, 71, 20));
        label_3 = new QLabel(GrowthLineVariousLayerDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(70, 70, 71, 16));
        label_4 = new QLabel(GrowthLineVariousLayerDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(230, 70, 51, 20));

        retranslateUi(GrowthLineVariousLayerDialog);
        QObject::connect(pushButton_ok, SIGNAL(clicked(bool)), GrowthLineVariousLayerDialog, SLOT(close()));
        QObject::connect(pushButton_quit, SIGNAL(clicked(bool)), GrowthLineVariousLayerDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(GrowthLineVariousLayerDialog);
    } // setupUi

    void retranslateUi(QDialog *GrowthLineVariousLayerDialog)
    {
        GrowthLineVariousLayerDialog->setWindowTitle(QApplication::translate("GrowthLineVariousLayerDialog", "\347\224\237\351\225\277\347\272\277\345\217\230\345\261\202\345\216\232\346\233\262\351\235\242\345\210\206\345\261\202\345\217\202\346\225\260", nullptr));
        pushButton_ok->setText(QApplication::translate("GrowthLineVariousLayerDialog", "\347\241\256\345\256\232", nullptr));
        label->setText(QApplication::translate("GrowthLineVariousLayerDialog", "mm", nullptr));
        pushButton_quit->setText(QApplication::translate("GrowthLineVariousLayerDialog", "\345\217\226\346\266\210", nullptr));
        lineEdit_1->setText(QApplication::translate("GrowthLineVariousLayerDialog", "4.0", nullptr));
        label_2->setText(QApplication::translate("GrowthLineVariousLayerDialog", "\346\234\200\345\244\247\345\261\202\351\253\230\357\274\232", nullptr));
        lineEdit_2->setText(QApplication::translate("GrowthLineVariousLayerDialog", "2.0", nullptr));
        label_3->setText(QApplication::translate("GrowthLineVariousLayerDialog", "\346\234\200\345\260\217\345\261\202\351\253\230\357\274\232", nullptr));
        label_4->setText(QApplication::translate("GrowthLineVariousLayerDialog", "mm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GrowthLineVariousLayerDialog: public Ui_GrowthLineVariousLayerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROWTHLINEVARIOUSLAYERDIALOG_H
