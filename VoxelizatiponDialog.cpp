#include "VoxelizatiponDialog.h"

VoxelizatiponDialog::VoxelizatiponDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	parameter = 0.1;//默认值
}

VoxelizatiponDialog::~VoxelizatiponDialog()
{
}

void VoxelizatiponDialog::on_pushButton_ok_clicked()
{
	parameter = ui.lineEdit_2->text().toDouble();//获取对话框中的参数
	emit sendData(parameter);
}
