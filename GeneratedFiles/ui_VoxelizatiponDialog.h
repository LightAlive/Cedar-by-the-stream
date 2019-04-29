/********************************************************************************
** Form generated from reading UI file 'VoxelizatiponDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOXELIZATIPONDIALOG_H
#define UI_VOXELIZATIPONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_VoxelizatiponDialog
{
public:
    QPushButton *pushButton_ok;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QPushButton *pushButton_quit;
    QLabel *label;

    void setupUi(QDialog *VoxelizatiponDialog)
    {
        if (VoxelizatiponDialog->objectName().isEmpty())
            VoxelizatiponDialog->setObjectName(QString::fromUtf8("VoxelizatiponDialog"));
        VoxelizatiponDialog->resize(238, 138);
        pushButton_ok = new QPushButton(VoxelizatiponDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(20, 100, 75, 23));
        lineEdit_2 = new QLineEdit(VoxelizatiponDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(100, 40, 61, 20));
        label_2 = new QLabel(VoxelizatiponDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 40, 71, 16));
        pushButton_quit = new QPushButton(VoxelizatiponDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(140, 100, 75, 23));
        label = new QLabel(VoxelizatiponDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(173, 40, 41, 20));

        retranslateUi(VoxelizatiponDialog);
        QObject::connect(pushButton_quit, SIGNAL(clicked(bool)), VoxelizatiponDialog, SLOT(close()));
        QObject::connect(pushButton_ok, SIGNAL(clicked(bool)), VoxelizatiponDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(VoxelizatiponDialog);
    } // setupUi

    void retranslateUi(QDialog *VoxelizatiponDialog)
    {
        VoxelizatiponDialog->setWindowTitle(QApplication::translate("VoxelizatiponDialog", "\344\275\223\347\264\240\345\214\226\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        pushButton_ok->setText(QApplication::translate("VoxelizatiponDialog", "\347\241\256\345\256\232", nullptr));
        lineEdit_2->setText(QApplication::translate("VoxelizatiponDialog", "0.5", nullptr));
        label_2->setText(QApplication::translate("VoxelizatiponDialog", "\344\275\223\347\264\240\345\214\226\347\262\276\345\272\246\357\274\232", nullptr));
        pushButton_quit->setText(QApplication::translate("VoxelizatiponDialog", "\345\217\226\346\266\210", nullptr));
        label->setText(QApplication::translate("VoxelizatiponDialog", "mm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VoxelizatiponDialog: public Ui_VoxelizatiponDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOXELIZATIPONDIALOG_H
