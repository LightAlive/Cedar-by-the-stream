#pragma once
#include<iostream>
#include<vtkCylinderSource.h>
#include <vtkLineSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkTubeFilter.h>
#include<Windows.h>
using namespace std;
typedef unsigned _int8 Byte;

class WeldingTorch
{
	int H = 30, D = 10, h = 10, d = 4;//焊枪尺寸参数
	int x0 = 50, y0 = 50, z0 = 100;//初始坐标
	double lx = 0, ly = 0;//焊枪轴方向
	const double Pi = 3.1415926, j = Pi / 24;
	vtkSmartPointer<vtkLineSource> axisLine1;
	vtkSmartPointer<vtkTubeFilter> tubeFilter1;
	vtkSmartPointer<vtkPolyDataMapper> weldingMapper1;
	vtkSmartPointer<vtkLineSource> axisLine2;
	vtkSmartPointer<vtkTubeFilter> tubeFilter2;
	vtkSmartPointer<vtkPolyDataMapper> weldingMapper2;
public:
	int f[5000][3]{ 0 };
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


	bool Detection(Byte a[200][200][200]);  //干涉检测，保存干涉点集合
	void Dispose(Byte a[200][200][200]);                      //干涉处理

};