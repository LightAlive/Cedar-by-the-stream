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

	vtkSmartPointer<vtkRenderer> m_pRenderer;//渲染器
	vtkSmartPointer<vtkSTLReader> reader;//保存读取的文件
	vtkSmartPointer<vtkActor> actorSTL;
	vtkSmartPointer<vtkActor> actorVoxel;

	vtkSmartPointer<vtkActor> actorWelding1;        //添加焊枪
	vtkSmartPointer<vtkActor> actorWelding2;
	WeldingTorch weld;
	double **arr;                      //保存路径点和机器人姿态

	CZ3DRepresentation *rep;//体素化模型
	CCubef fcube;


	//曲面分层代码
	void makeCurvedLayer(double m_layerHigh);
	void creatCurvedLayer();
	void creatCurve();
	void creatSolidPath();
	void glDrawCurve(Byte *buffer, vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells);
	void glDrawPath(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells, int x, int y, int z);
	void glDrawCurve2(Byte *buffer, CZ3DRepresentation *rep1,vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells);


	//构建存放路径数据的数组
	SurfacePath m_surfacePath;
	SolidPath m_solidPath;

	typedef std::vector<Byte*> Buff_CurvedLayer;//用以存放各曲面层的buff
	Buff_CurvedLayer buff_curvedLayer;



private slots:
	void on_action_Tool_triggered();//这样命名的好处是，直接实现Action中信号和槽的连接.不需要connect
	void on_actionZigzag_triggered();
	void on_action_Gcode_triggered();
	void on_action_Robot_triggered();
	void on_action_Outline_triggered();
	void on_action_Voxel_triggered();
	void on_action_STL_triggered();
	void on_action_Simulation_triggered();
	void on_action_IsochoricLayer_triggered();
	void on_action_VariableThicknessLayer_triggered();

	void on_actionDetection_triggered();//干涉处理
	void on_actionRobot_triggered();//生成机器人.src文件
	void on_action_dat_triggered();//生成机器人.dat文件

	void voxelizationButten_clicked();//实现STL文件体素化


	void ToolLayerButton_clicked();//获取曲面刀具分层层高信息
	void ZigzagButton_clicked();//获取填充路径的间隔
	void OutlineButton_clicked();//获取轮廓偏置路径
	void GrowthLineIsochoricLayerButton_clicked();//获取生长线等层厚曲面分层的层高信息
	void GrowthLineVariousLayerButton_clicked();//获取生长线变层厚曲面分层的层高信息
	void RobotButton_clicked();//保存输出的路经点为机器人离线文件
	void robotDATInitDunp(ostream &p_os);
	void robotSRCInitDunp(ostream &p_os);
	void robotSRCDunp(ostream &p_os, int p_size);
	void GcodeButton_clicked();//保存输出的路经点为数控代码

};
