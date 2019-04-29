#include "GcodeDialog.h"

GcodeDialog::GcodeDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

GcodeDialog::~GcodeDialog()
{
}


void GcodeDialog::on_pushButton_ok_clicked()
{
	speedParameter = ui.lineEdit->text().toDouble();
	emit sendData(speedParameter);
}