#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testnew.h"

class testnew : public QMainWindow
{
	Q_OBJECT

public:
	testnew(QWidget *parent = Q_NULLPTR);

private:
	Ui::testnewClass ui;
};
