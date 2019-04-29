#include "OutlineDialog.h"

OutlineDialog::OutlineDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

OutlineDialog::~OutlineDialog()
{
}

void OutlineDialog::on_pushButton_ok_clicked()
{
	intervalParameter = ui.lineEdit->text().toDouble();
	emit sendData(intervalParameter);
}
