#pragma once

#include <QDialog>
#include "ui_GrowthLineVariousLayerDialog.h"

class GrowthLineVariousLayerDialog : public QDialog
{
	Q_OBJECT

public:
	GrowthLineVariousLayerDialog(QWidget *parent = Q_NULLPTR);
	~GrowthLineVariousLayerDialog();

	double dOfMaxLayer;//最大层高
	double dOfMinLayer;//最小层高

signals:
	void sendData(double data1, double data2);

private:
	Ui::GrowthLineVariousLayerDialog ui;
	private slots:
	void on_pushButton_ok_clicked();
};
