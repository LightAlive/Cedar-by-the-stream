#pragma once

#include <QDialog>
#include "ui_VoxelizatiponDialog.h"

class VoxelizatiponDialog : public QDialog
{
	Q_OBJECT

public:
	VoxelizatiponDialog(QWidget *parent = Q_NULLPTR);
	~VoxelizatiponDialog();

	double parameter;//�������ݶԻ���Ĳ�����������

signals:
	void sendData(double data);


private:
	Ui::VoxelizatiponDialog ui;

	private slots:
	void on_pushButton_ok_clicked();

};
