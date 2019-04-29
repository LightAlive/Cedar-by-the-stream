#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_test2.h"
#include "ZigzagDialog.h"
#include "OutlineDialog.h"
#include "VoxelizatiponDialog.h"
#include "GcodeDialog.h"
#include "RobotDialog.h"
#include "ToolLayerDialog.h"
#include "stdafx.h"
#include "3dPoint.h"
#include "GrowthLineLayerDialog.h"
#include "GrowthLineVariousLayerDialog.h"

#include<Windows.h>
#include<fstream>
#include "WeldingTorch.h"
class ZigzagDialog;
class OutlineDialog;
class VoxelizatiponDialog;
class GcodeDialog;
class RobotDialog;
class ToolLayerDialog;
class GrowthLineLayerDialog;
class GrowthLineVariousLayerDialog;

class WeldingTorch;

class test2 : public QMainWindow
{
	Q_OBJECT

public:
	test2(QWidget *parent = Q_NULLPTR);

	
	
private:
	Ui::test2Class ui;

	ZigzagDialog *zigzagDialog;
	OutlineDialog *outlineDialog;
	VoxelizatiponDialog *voxelizationDialog;
	GcodeDialog *gcodeDialog;
	RobotDialog *robotDialog;
	ToolLayerDialog *toolLayerDialog;
	GrowthLineLayerDialog *growthLineLayerDialog;
	GrowthLineVariousLayerDialog *growthLineVariousLayerDialog;

	vtkSmartPointer<vtkRenderer> m_pRenderer;//��Ⱦ��
	vtkSmartPointer<vtkSTLReader> reader;//�����ȡ���ļ�
	vtkSmartPointer<vtkActor> actorSTL;
	vtkSmartPointer<vtkActor> actorVoxel;

	vtkSmartPointer<vtkActor> actorWelding1;        //��Ӻ�ǹ
	vtkSmartPointer<vtkActor> actorWelding2;
	WeldingTorch weld;
	double **arr;                      //����·����ͻ�������̬

	CZ3DRepresentation *rep;//���ػ�ģ��
	CCubef fcube;


	//����ֲ����
	void makeCurvedLayer(double m_layerHigh);
	void creatCurvedLayer();
	void creatCurve();
	void creatSolidPath();
	void glDrawCurve(Byte *buffer, vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells);
	void glDrawPath(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, int x, int y, int z);
	void glDrawCurve2(Byte *buffer, CZ3DRepresentation *rep1,vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells);


	//�������·�����ݵ�����
	SurfacePath m_surfacePath;
	SolidPath m_solidPath;

	typedef std::vector<Byte*> Buff_CurvedLayer;//���Դ�Ÿ�������buff
	Buff_CurvedLayer buff_curvedLayer;



private slots:
	void on_action_Tool_triggered();//���������ĺô��ǣ�ֱ��ʵ��Action���źźͲ۵�����.����Ҫconnect
	void on_actionZigzag_triggered();
	void on_action_Gcode_triggered();
	void on_action_Robot_triggered();
	void on_action_Outline_triggered();
	void on_action_Voxel_triggered();
	void on_action_STL_triggered();
	void on_action_Simulation_triggered();
	void on_action_IsochoricLayer_triggered();
	void on_action_VariableThicknessLayer_triggered();

	void on_actionDetection_triggered();//���洦��
	void on_actionRobot_triggered();//���ɻ�����.src�ļ�
	void on_action_dat_triggered();//���ɻ�����.dat�ļ�

	void voxelizationButten_clicked();//ʵ��STL�ļ����ػ�


	void ToolLayerButton_clicked();//��ȡ���浶�߷ֲ�����Ϣ
	void ZigzagButton_clicked();//��ȡ���·���ļ��
	void OutlineButton_clicked();//��ȡ����ƫ��·��
	void GrowthLineIsochoricLayerButton_clicked();//��ȡ�����ߵȲ������ֲ�Ĳ����Ϣ
	void GrowthLineVariousLayerButton_clicked();//��ȡ�����߱�������ֲ�Ĳ����Ϣ
	void RobotButton_clicked();//���������·����Ϊ�����������ļ�
	void robotDATInitDunp(ostream &p_os);
	void robotSRCInitDunp(ostream &p_os);
	void robotSRCDunp(ostream &p_os, int p_size);
	void GcodeButton_clicked();//���������·����Ϊ���ش���

};
