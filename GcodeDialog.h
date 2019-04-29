#pragma once

#include <QDialog>
#include "ui_GcodeDialog.h"

class GcodeDialog : public QDialog
{
	Q_OBJECT

public:
	GcodeDialog(QWidget *parent = Q_NULLPTR);
	~GcodeDialog();

	double speedParameter;

signals:
	void sendData(double data);

private:
	Ui::GcodeDialog ui;

	private slots:
	void on_pushButton_ok_clicked();
};
