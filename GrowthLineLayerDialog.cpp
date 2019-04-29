#include "GrowthLineLayerDialog.h"

GrowthLineLayerDialog::GrowthLineLayerDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

GrowthLineLayerDialog::~GrowthLineLayerDialog()
{
}

void GrowthLineLayerDialog::on_pushButton_ok_clicked()
{
	dOfLayer = ui.lineEdit_2->text().toDouble();
	emit sendData(dOfLayer);
}
