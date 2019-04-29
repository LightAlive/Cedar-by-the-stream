#pragma once
#include<iostream>
#include<functional>

#include "vtkActor.h"
#include "vtkPolyData.h"//VTK所用的头文件
#include "vtkSmartPointer.h"
#include "vtkCutter.h"
#include "vtkPoints.h"
#include "vtkLine.h"
#include "vtkPlane.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkSTLReader.h"
#include "vtkCutter.h"
#include "vtkPlane.h"
#include "vtkPlaneSource.h"
#include "vtkPolyData.h"
#include "vtkProperty.h"
#include "vtkPoints.h"
#include "vtkPolygon.h"
#include "vtkIntersectionPolyDataFilter.h"
#include "vtkLineSource.h"
#include "vtkSphereSource.h"
#include "vtkBooleanOperationPolyDataFilter.h"
#include "vtkCellData.h"
#include "vtkCubeSource.h"
#include "vtkBoundingBox.h"
#include "vtkCellArray.h"
#include "vtkImageData.h"
#include "vtkVoxel.h"
#include "vtkVoxelModeller.h"
#include "vtkImageMapper.h"
#include "vtkImageActor.h"
#include "vtkImageMapper3D.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkAxesActor.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkRegularPolygonSource.h"
#include "vtkAppendPolyData.h"
#include "vtkArcSource.h"
#include "vtkTubeFilter.h"
#include "vtkCellType.h"
#include "vtkProperty.h"

#include"3DRepresentation.h"
using namespace std;
typedef unsigned _int8 Byte;


//焊枪定义
class WeldingTorch
{
	int H = 30, D = 10, h = 10, d = 4;//焊枪尺寸参数
	int x0 = 50, y0 = 50, z0 = 100;//初始坐标
	double lx = 0, ly = 0;//焊枪轴方向
	const double Pi = 3.1415926, j = Pi / 48;
	
	

	vtkSmartPointer<vtkLineSource> axisLine1;
	vtkSmartPointer<vtkTubeFilter> tubeFilter1;
	vtkSmartPointer<vtkPolyDataMapper> weldingMapper1;
	vtkSmartPointer<vtkLineSource> axisLine2;
	vtkSmartPointer<vtkTubeFilter> tubeFilter2;
	vtkSmartPointer<vtkPolyDataMapper> weldingMapper2;
public:
	int f[5000][2];
	int getx() { return x0; }
	int gety() { return y0; }
	int getz() { return z0; }
	double getlx() { return lx; }
	double getly() { return ly; }

	
	WeldingTorch();
	void setx(int xx) { x0 = xx; }
	void sety(int yy) { y0 = yy; }
	void setz(int zz) { z0 = zz; }
	void setlx(double lxx) { lx = lxx; }
	void setly(double lyy) { ly = lyy; }
	vtkSmartPointer<vtkPolyDataMapper> draw1();
	vtkSmartPointer<vtkPolyDataMapper> draw2();

	
	
	bool Detection(Byte*a,int m_yCoefficient,int m_zCoefficient,unsigned long long sum);  //干涉检测，保存干涉点集合
	void Dispose(Byte *a, int m_yCoefficient, int m_zCoefficient, unsigned long long sum);                      //干涉处理


};
