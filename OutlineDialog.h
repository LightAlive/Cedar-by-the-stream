#pragma once

#include <QDialog>
#include "ui_OutlineDialog.h"

class OutlineDialog : public QDialog
{
	Q_OBJECT

public:
	OutlineDialog(QWidget *parent = Q_NULLPTR);
	~OutlineDialog();

	double intervalParameter;

signals:
	void sendData(double data);

private:
	Ui::OutlineDialog ui;
	
	private slots:
	void on_pushButton_ok_clicked();
};
