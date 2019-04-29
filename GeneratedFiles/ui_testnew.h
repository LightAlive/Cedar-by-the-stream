/********************************************************************************
** Form generated from reading UI file 'testnew.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTNEW_H
#define UI_TESTNEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_testnewClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *testnewClass)
    {
        if (testnewClass->objectName().isEmpty())
            testnewClass->setObjectName(QString::fromUtf8("testnewClass"));
        testnewClass->resize(600, 400);
        menuBar = new QMenuBar(testnewClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        testnewClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(testnewClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        testnewClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(testnewClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        testnewClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(testnewClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        testnewClass->setStatusBar(statusBar);

        retranslateUi(testnewClass);

        QMetaObject::connectSlotsByName(testnewClass);
    } // setupUi

    void retranslateUi(QMainWindow *testnewClass)
    {
        testnewClass->setWindowTitle(QApplication::translate("testnewClass", "testnew", nullptr));
    } // retranslateUi

};

namespace Ui {
    class testnewClass: public Ui_testnewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTNEW_H
