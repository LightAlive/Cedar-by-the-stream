#pragma once

#include <QDialog>
#include "ui_ToolLayerDialog.h"

class ToolLayerDialog : public QDialog
{
	Q_OBJECT

public:
	ToolLayerDialog(QWidget *parent = Q_NULLPTR);
	~ToolLayerDialog();

	double dOfLayer;//²ã¸ß

signals:
	void sendData(double data);

private:
	Ui::ToolLayerDialog ui;

	private slots:
	void on_pushButton_ok_clicked();
};
