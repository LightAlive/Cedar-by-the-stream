/********************************************************************************
** Form generated from reading UI file 'test2.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST2_H
#define UI_TEST2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_test2Class
{
public:
    QAction *action_STL;
    QAction *action_Voxel;
    QAction *action_Tool;
    QAction *actionZigzag;
    QAction *action_Outline;
    QAction *action_Simulation;
    QAction *action_Gcode;
    QAction *action_Robot;
    QAction *action_IsochoricLayer;
    QAction *action_VariableThicknessLayer;
    QAction *actionDetection;
    QAction *actionRobot;
    QAction *action_dat;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVTKWidget *qvtkWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_7;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menu_6;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *test2Class)
    {
        if (test2Class->objectName().isEmpty())
            test2Class->setObjectName(QString::fromUtf8("test2Class"));
        test2Class->resize(623, 510);
        action_STL = new QAction(test2Class);
        action_STL->setObjectName(QString::fromUtf8("action_STL"));
        action_Voxel = new QAction(test2Class);
        action_Voxel->setObjectName(QString::fromUtf8("action_Voxel"));
        action_Tool = new QAction(test2Class);
        action_Tool->setObjectName(QString::fromUtf8("action_Tool"));
        actionZigzag = new QAction(test2Class);
        actionZigzag->setObjectName(QString::fromUtf8("actionZigzag"));
        action_Outline = new QAction(test2Class);
        action_Outline->setObjectName(QString::fromUtf8("action_Outline"));
        action_Simulation = new QAction(test2Class);
        action_Simulation->setObjectName(QString::fromUtf8("action_Simulation"));
        action_Gcode = new QAction(test2Class);
        action_Gcode->setObjectName(QString::fromUtf8("action_Gcode"));
        action_Robot = new QAction(test2Class);
        action_Robot->setObjectName(QString::fromUtf8("action_Robot"));
        action_IsochoricLayer = new QAction(test2Class);
        action_IsochoricLayer->setObjectName(QString::fromUtf8("action_IsochoricLayer"));
        action_VariableThicknessLayer = new QAction(test2Class);
        action_VariableThicknessLayer->setObjectName(QString::fromUtf8("action_VariableThicknessLayer"));
        actionDetection = new QAction(test2Class);
        actionDetection->setObjectName(QString::fromUtf8("actionDetection"));
        actionDetection->setCheckable(false);
        actionRobot = new QAction(test2Class);
        actionRobot->setObjectName(QString::fromUtf8("actionRobot"));
        action_dat = new QAction(test2Class);
        action_dat->setObjectName(QString::fromUtf8("action_dat"));
        centralWidget = new QWidget(test2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        qvtkWidget = new QVTKWidget(centralWidget);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));

        gridLayout->addWidget(qvtkWidget, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        test2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(test2Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 623, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_7 = new QMenu(menu_3);
        menu_7->setObjectName(QString::fromUtf8("menu_7"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        menu_5 = new QMenu(menuBar);
        menu_5->setObjectName(QString::fromUtf8("menu_5"));
        menu_6 = new QMenu(menuBar);
        menu_6->setObjectName(QString::fromUtf8("menu_6"));
        test2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(test2Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        test2Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(test2Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        test2Class->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menuBar->addAction(menu_5->menuAction());
        menuBar->addAction(menu_6->menuAction());
        menu->addAction(action_STL);
        menu_2->addAction(action_Voxel);
        menu_2->addAction(actionDetection);
        menu_2->addAction(actionRobot);
        menu_2->addAction(action_dat);
        menu_3->addAction(action_Tool);
        menu_3->addAction(menu_7->menuAction());
        menu_7->addAction(action_IsochoricLayer);
        menu_7->addAction(action_VariableThicknessLayer);
        menu_4->addAction(actionZigzag);
        menu_4->addAction(action_Outline);
        menu_5->addAction(action_Simulation);
        menu_6->addAction(action_Gcode);
        menu_6->addAction(action_Robot);

        retranslateUi(test2Class);

        QMetaObject::connectSlotsByName(test2Class);
    } // setupUi

    void retranslateUi(QMainWindow *test2Class)
    {
        test2Class->setWindowTitle(QApplication::translate("test2Class", "\346\233\262\351\235\242\345\210\206\345\261\202\345\210\266\351\200\240\345\267\245\350\211\272\350\247\204\345\210\222\350\275\257\344\273\266", nullptr));
        action_STL->setText(QApplication::translate("test2Class", "\346\211\223\345\274\200STL\346\226\207\344\273\266", nullptr));
        action_Voxel->setText(QApplication::translate("test2Class", "\347\224\237\346\210\220\344\275\223\347\264\240\346\250\241\345\236\213", nullptr));
        action_Tool->setText(QApplication::translate("test2Class", "\345\237\272\344\272\216\346\233\262\351\235\242\345\201\217\347\275\256\347\232\204\346\233\262\351\235\242\345\210\206\345\261\202", nullptr));
        actionZigzag->setText(QApplication::translate("test2Class", "Z\345\255\227\345\275\242\350\267\257\345\276\204", nullptr));
        action_Outline->setText(QApplication::translate("test2Class", "\350\275\256\345\273\223\350\267\257\345\276\204", nullptr));
        action_Simulation->setText(QApplication::translate("test2Class", "\345\210\266\351\200\240\350\267\257\345\276\204\345\212\250\346\200\201\346\250\241\346\213\237", nullptr));
        action_Gcode->setText(QApplication::translate("test2Class", "G\344\273\243\347\240\201", nullptr));
        action_Robot->setText(QApplication::translate("test2Class", "\346\234\272\345\231\250\344\272\272\347\246\273\347\272\277\347\274\226\347\250\213\346\226\207\344\273\266", nullptr));
        action_IsochoricLayer->setText(QApplication::translate("test2Class", "\347\255\211\345\261\202\345\216\232\345\210\206\345\261\202", nullptr));
        action_VariableThicknessLayer->setText(QApplication::translate("test2Class", "\345\217\230\345\261\202\345\216\232\345\210\206\345\261\202", nullptr));
        actionDetection->setText(QApplication::translate("test2Class", "\345\271\262\346\266\211\346\243\200\346\265\213", nullptr));
        actionRobot->setText(QApplication::translate("test2Class", "\347\224\237\346\210\220\346\234\272\345\231\250\344\272\272.src\346\226\207\344\273\266", nullptr));
        action_dat->setText(QApplication::translate("test2Class", "\347\224\237\346\210\220\346\234\272\345\231\250\344\272\272.dat\346\226\207\344\273\266", nullptr));
        menu->setTitle(QApplication::translate("test2Class", "\346\211\223\345\274\200", nullptr));
        menu_2->setTitle(QApplication::translate("test2Class", "\344\275\223\347\264\240\345\214\226", nullptr));
        menu_3->setTitle(QApplication::translate("test2Class", "\346\233\262\351\235\242\345\210\206\345\261\202", nullptr));
        menu_7->setTitle(QApplication::translate("test2Class", "\345\237\272\344\272\216\347\224\237\351\225\277\347\272\277\347\232\204\346\233\262\351\235\242\345\210\206\345\261\202", nullptr));
        menu_4->setTitle(QApplication::translate("test2Class", "\350\267\257\345\276\204\345\241\253\345\205\205", nullptr));
        menu_5->setTitle(QApplication::translate("test2Class", "\345\210\266\351\200\240\346\250\241\346\213\237", nullptr));
        menu_6->setTitle(QApplication::translate("test2Class", "\350\276\223\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class test2Class: public Ui_test2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST2_H
