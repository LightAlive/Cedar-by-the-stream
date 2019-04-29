#include "ZigzagDialog.h"

ZigzagDialog::ZigzagDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

ZigzagDialog::~ZigzagDialog()
{
}

void ZigzagDialog::on_pushButton_ok_clicked()
{
	intervalParameter = ui.lineEdit->text().toDouble();
	emit sendData(intervalParameter);
}
