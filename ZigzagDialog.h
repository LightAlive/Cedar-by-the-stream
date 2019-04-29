#pragma once

#include <QDialog>
#include "ui_ZigzagDialog.h"

class ZigzagDialog : public QDialog
{
	Q_OBJECT

public:
	ZigzagDialog(QWidget *parent = Q_NULLPTR);
	~ZigzagDialog();

	double intervalParameter;

signals:
	void sendData(double data);

private:
	Ui::ZigzagDialog ui;

	private slots:
	void on_pushButton_ok_clicked();
};
