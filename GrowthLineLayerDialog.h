#pragma once

#include <QDialog>
#include "ui_GrowthLineLayerDialog.h"

class GrowthLineLayerDialog : public QDialog
{
	Q_OBJECT

public:
	GrowthLineLayerDialog(QWidget *parent = Q_NULLPTR);
	~GrowthLineLayerDialog();

	double dOfLayer;//²ã¸ß

signals:
	void sendData(double data);

private:
	Ui::GrowthLineLayerDialog ui;
	
	private slots:
	void on_pushButton_ok_clicked();
};
