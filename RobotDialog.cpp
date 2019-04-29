#include "RobotDialog.h"

RobotDialog::RobotDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

RobotDialog::~RobotDialog()
{
}

void RobotDialog::on_pushButton_ok_clicked()
{
	m_speedData = ui.lineEdit->text().toDouble();
	m_baseNumber = ui.lineEdit_2->text().toInt();
	m_JOBnumber = ui.lineEdit_3->text().toInt();
	m_A = ui.lineEdit_4->text().toDouble();
	m_B = ui.lineEdit_5->text().toDouble();
	m_C = ui.lineEdit_6->text().toDouble();
	m_S = ui.lineEdit_7->text().toInt();
	m_T = ui.lineEdit_8->text().toInt();

	emit sendData(m_speedData, m_baseNumber, m_JOBnumber, m_A, m_B, m_C, m_S, m_T);

}
