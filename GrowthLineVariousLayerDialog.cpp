#include "GrowthLineVariousLayerDialog.h"

GrowthLineVariousLayerDialog::GrowthLineVariousLayerDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

GrowthLineVariousLayerDialog::~GrowthLineVariousLayerDialog()
{
}

void GrowthLineVariousLayerDialog::on_pushButton_ok_clicked()
{
	dOfMaxLayer = ui.lineEdit_1->text().toDouble();
	dOfMinLayer = ui.lineEdit_2->text().toDouble();
	emit sendData(dOfMaxLayer, dOfMinLayer);
}
