#include "VoxelizatiponDialog.h"

VoxelizatiponDialog::VoxelizatiponDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	parameter = 0.1;//Ĭ��ֵ
}

VoxelizatiponDialog::~VoxelizatiponDialog()
{
}

void VoxelizatiponDialog::on_pushButton_ok_clicked()
{
	parameter = ui.lineEdit_2->text().toDouble();//��ȡ�Ի����еĲ���
	emit sendData(parameter);
}
