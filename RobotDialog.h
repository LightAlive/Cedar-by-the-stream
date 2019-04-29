#pragma once

#include <QDialog>
#include "ui_RobotDialog.h"

class RobotDialog : public QDialog
{
	Q_OBJECT

signals:
	void sendData(double speedData, int baseNumber, int JOBnumber, double A, double B, double C, int S, int T);


public:
	RobotDialog(QWidget *parent = Q_NULLPTR);
	~RobotDialog();

	double m_speedData;
	int m_baseNumber; 
	int m_JOBnumber; 
	double m_A; 
	double m_B; 
	double m_C; 
	int m_S; 
	int m_T;

private:
	Ui::RobotDialog ui;

	private slots:
	void on_pushButton_ok_clicked();
};
