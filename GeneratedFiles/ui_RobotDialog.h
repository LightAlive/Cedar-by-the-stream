/********************************************************************************
** Form generated from reading UI file 'RobotDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROBOTDIALOG_H
#define UI_ROBOTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RobotDialog
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QFrame *line;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLabel *label_7;
    QLineEdit *lineEdit_6;
    QLabel *label_8;
    QLineEdit *lineEdit_7;
    QLabel *label_9;
    QLineEdit *lineEdit_8;
    QLabel *label_10;
    QPushButton *pushButton_ok;
    QPushButton *pushButton_quit;

    void setupUi(QDialog *RobotDialog)
    {
        if (RobotDialog->objectName().isEmpty())
            RobotDialog->setObjectName(QString::fromUtf8("RobotDialog"));
        RobotDialog->resize(423, 278);
        label = new QLabel(RobotDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 20, 54, 16));
        lineEdit = new QLineEdit(RobotDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(110, 20, 91, 20));
        label_2 = new QLabel(RobotDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(203, 22, 51, 16));
        label_3 = new QLabel(RobotDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 50, 54, 12));
        lineEdit_2 = new QLineEdit(RobotDialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(110, 50, 91, 20));
        label_4 = new QLabel(RobotDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(230, 50, 71, 16));
        lineEdit_3 = new QLineEdit(RobotDialog);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(300, 50, 91, 20));
        line = new QFrame(RobotDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 90, 401, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_5 = new QLabel(RobotDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 110, 91, 16));
        label_6 = new QLabel(RobotDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(40, 140, 21, 16));
        lineEdit_4 = new QLineEdit(RobotDialog);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(60, 140, 71, 20));
        lineEdit_5 = new QLineEdit(RobotDialog);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(190, 140, 71, 20));
        label_7 = new QLabel(RobotDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(170, 140, 21, 16));
        lineEdit_6 = new QLineEdit(RobotDialog);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(320, 140, 71, 20));
        label_8 = new QLabel(RobotDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(300, 140, 21, 16));
        lineEdit_7 = new QLineEdit(RobotDialog);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(60, 180, 71, 20));
        label_9 = new QLabel(RobotDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(40, 180, 21, 16));
        lineEdit_8 = new QLineEdit(RobotDialog);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(190, 180, 71, 20));
        label_10 = new QLabel(RobotDialog);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(170, 180, 21, 16));
        pushButton_ok = new QPushButton(RobotDialog);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));
        pushButton_ok->setGeometry(QRect(100, 230, 75, 23));
        pushButton_quit = new QPushButton(RobotDialog);
        pushButton_quit->setObjectName(QString::fromUtf8("pushButton_quit"));
        pushButton_quit->setGeometry(QRect(220, 230, 75, 23));

        retranslateUi(RobotDialog);
        QObject::connect(pushButton_ok, SIGNAL(clicked(bool)), RobotDialog, SLOT(close()));
        QObject::connect(pushButton_quit, SIGNAL(clicked(bool)), RobotDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(RobotDialog);
    } // setupUi

    void retranslateUi(QDialog *RobotDialog)
    {
        RobotDialog->setWindowTitle(QApplication::translate("RobotDialog", "RobotDialog", nullptr));
        label->setText(QApplication::translate("RobotDialog", "\346\211\223\345\215\260\351\200\237\345\272\246\357\274\232", nullptr));
        label_2->setText(QApplication::translate("RobotDialog", "(mm/s)", nullptr));
        label_3->setText(QApplication::translate("RobotDialog", "\345\237\272\345\235\220\346\240\207\345\217\267\357\274\232", nullptr));
        label_4->setText(QApplication::translate("RobotDialog", "\347\204\212\346\234\272JOB\345\217\267\357\274\232", nullptr));
        label_5->setText(QApplication::translate("RobotDialog", "\346\234\272\345\231\250\344\272\272\345\247\277\346\200\201\350\256\276\347\275\256", nullptr));
        label_6->setText(QApplication::translate("RobotDialog", "A\357\274\232", nullptr));
        lineEdit_5->setText(QString());
        label_7->setText(QApplication::translate("RobotDialog", "B\357\274\232", nullptr));
        label_8->setText(QApplication::translate("RobotDialog", "C\357\274\232", nullptr));
        lineEdit_7->setText(QString());
        label_9->setText(QApplication::translate("RobotDialog", "S\357\274\232", nullptr));
        label_10->setText(QApplication::translate("RobotDialog", "T\357\274\232", nullptr));
        pushButton_ok->setText(QApplication::translate("RobotDialog", "\347\241\256\345\256\232", nullptr));
        pushButton_quit->setText(QApplication::translate("RobotDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RobotDialog: public Ui_RobotDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROBOTDIALOG_H
