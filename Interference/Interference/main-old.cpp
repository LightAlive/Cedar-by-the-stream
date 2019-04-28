#include<cmath>
#include <stdlib.h>
#include <iostream>
#include "WeldingTorch.h"


#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);

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
#include <iostream>
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
using namespace std;
const int N = 200, n = 1;

typedef unsigned __int8	Byte;

Byte a[N][N][N], c[N][N][N];
const double Pi = 3.1415926;


int Xmax = 200;
int Ymax = 200;
int Zmax = 200;
void glDraw(vtkSmartPointer<vtkPoints> ugpoints, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkCellArray> cells)
{
	int pointsId = 0;
	for (int z = 0; z < N / n; z++)
	{
		for (int y = 1; y < N / n; y++)
		{
			for (int x = 1; x < N / n; x++)
			{
				if (a[z][y][x] == 1)
				{
					cells->InsertNextCell(8);
					ugpoints->InsertNextPoint((x - 1), (y - 1), z);
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x), (y - 1), z);
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1), (y), z);
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x), (y), z);
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1), (y - 1), (z + 1));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x), (y - 1), (z + 1));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x - 1), (y), (z + 1));
					cells->InsertCellPoint(pointsId++);

					ugpoints->InsertNextPoint((x), (y), (z + 1));
					cells->InsertCellPoint(pointsId++);
				}
			}
		}
	}
	ug->SetPoints(ugpoints);
	ug->SetCells(VTK_VOXEL, cells);

	vtkSmartPointer<vtkDataSetMapper> ugmapper = vtkSmartPointer<vtkDataSetMapper>::New();
	vtkSmartPointer<vtkActor> ugactor = vtkSmartPointer<vtkActor>::New();
	ugmapper->SetInputData(ug);
	ugactor->SetMapper(ugmapper);
	ugactor->GetProperty()->SetColor(1, 1, 0);//设置实体颜色

	vtkSmartPointer<vtkRenderer>renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(ugactor);
	renderer->SetBackground(1, 1, 1);//设置背景颜色
	vtkSmartPointer<vtkRenderWindow>renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor>renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindow->SetSize(300, 300);
	renderWindow->Render();
	renderWindowInteractor->Start();
}

bool Detection(int x0, int y0, int z0, double lx, double ly)
{
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	for (int z = z0; z <= z0 + 40 * cos(lx)*cos(ly) + 10; z++) {
		if (ly >= 0 && lx >= 0) {
			for (int y = y0 - 5; y <= y0 + 5 + 40 * sin(lx)*cos(ly) + 10; y++) {
				for (int x = x0 - 5; x <= x0 + 5 + 40 * sin(ly)*cos(lx) + 10; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) - z2*sin(lx); z3 = y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if (((x3*x3 + y3*y3) <= 4 && z3 <= 10 && z3 >= 0 || (x3*x3 + y3*y3) <= 25 && z3 <= 40 && z3 >= 10)
						&& a[z][y][x] == 1) {                                                              //判断是否重合

						return false;

					}
				}
			}
		}
		if (ly >= 0 && lx < 0) {
			for (int y = y0 - 5 + 40 * sin(lx)*cos(ly) - 10; y <= y0 + 5; y++) {
				for (int x = x0 - 5; x <= x0 + 5 + 40 * sin(ly)*cos(lx) + 10; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) - z2*sin(lx); z3 = y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if (((x3*x3 + y3*y3) <= 4 && z3 <= 10 && z3 >= 0 || (x3*x3 + y3*y3) <= 25 && z3 <= 40 && z3 >= 10)
						&& a[z][y][x] == 1) {                                                              //判断是否重合
						return false;

					}
				}
			}
		}
		if (ly < 0 && lx >= 0) {
			for (int y = y0 - 5; y <= y0 + 5 + 40 * sin(lx)*cos(ly) + 10; y++) {
				for (int x = x0 - 5 + 40 * sin(ly)*cos(lx) - 10; x <= x0 + 5; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) - z2*sin(lx); z3 = y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if (((x3*x3 + y3*y3) <= 4 && z3 <= 10 && z3 >= 0 || (x3*x3 + y3*y3) <= 25 && z3 <= 40 && z3 >= 10)
						&& a[z][y][x] == 1) {                                                              //判断是否重合
						return false;

					}
				}
			}
		}
		if (ly < 0 && lx < 0) {
			for (int y = y0 - 5 + 40 * sin(lx)*cos(ly) - 10; y <= y0 + 5; y++) {
				for (int x = x0 - 5 + 40 * sin(ly)*cos(lx) - 10; x <= x0 + 5; x++) {
					x1 = x - x0; y1 = y - y0; z1 = z - z0;                              //平移
					x2 = x1*cos(ly) - z1*sin(ly); y2 = y1; z2 = x1*sin(ly) + z1*cos(ly);//沿y轴旋转
					x3 = x2; y3 = y2*cos(lx) - z2*sin(lx); z3 = y2*sin(lx) + z2*cos(lx);//沿x轴旋转
					if (((x3*x3 + y3*y3) <= 4 && z3 <= 10 && z3 >= 0 || (x3*x3 + y3*y3) <= 25 && z3 <= 40 && z3 >= 10)
						&& a[z][y][x] == 1) {                                                              //判断是否重合
						return false;

					}
				}
			}
		}
	}
	return true;
}
int main()
{
	double x1, y1, z1, x2, y2, z2,x3,y3,z3; int b[5000][3]{0},k=0;
	WeldingTorch w;
	

	for (int z = 0; z < N / n; z++) {

	for (int y = 0; y < N / n; y++) {
	for (int x = 0; x < N / n; x++) {
	if ((x==1||x==51||y==1||y==51||z==1)&&x<=51&&y<=51&&z<=51)
	a[z][y][x] = 1;

	}
	}

	}
	w.setx(49); w.sety(49); w.setz(2);
	w.setlx(0); w.setly(0);
	while (!w.Detection(a))
	    w.Dispose();
	
	
	cout << w.getlx() << ' ' << w.getly() << endl;
	int x0 = w.getx(), y0 = w.gety(), z0 =w.getz();
	
	

	for (int z = z0; z <= N ; z++) {

	for (int y = 3; y <= N; y++) {
	for (int x = 3; x <= N; x++) {
	//	if(x>=0&&y>=0&&z>=0)
	//a[z][y][x] = 1;
	x1 = x - x0; y1 = y -y0; z1 = z - z0;


	x2 = x1*cos(w.getly()) - z1*sin(w.getly()); y2 = y1; z2 = x1*sin(w.getly()) + z1*cos(w.getly());//沿y轴旋转
	x3 = x2; y3 = y2*cos(w.getlx()) + z2*sin(w.getlx()); z3 = -y2*sin(w.getlx()) + z2*cos(w.getlx());//沿x轴旋转
	if ((x3*x3 + y3*y3) <= 4 && z3 <= 10 && z3 >= 0 || (x3*x3 + y3*y3) <= 25 && z3 <= 40 && z3 >= 10) {

	a[z][y][x] = 1;
	}

	}
	}

	}
	
    vtkSmartPointer<vtkPoints>spherePoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray>sphereCell = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkUnstructuredGrid>sphereGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

	glDraw(spherePoints, sphereGrid, sphereCell);
	

	

	return 0;
}