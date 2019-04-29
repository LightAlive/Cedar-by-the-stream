#include "ToolLayerDialog.h"

ToolLayerDialog::ToolLayerDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	dOfLayer = 1.0;
}

ToolLayerDialog::~ToolLayerDialog()
{
}

void ToolLayerDialog::on_pushButton_ok_clicked()
{
	dOfLayer = ui.lineEdit_2->text().toDouble();
	emit sendData(dOfLayer);
}
