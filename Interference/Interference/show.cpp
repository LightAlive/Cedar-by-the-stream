#include<cmath>
#include <stdlib.h>
#include <iostream>
#include "WeldingTorch.h"

#include<fstream>
#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
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

int main()
{



	//创建边界
	int pointsId = 0;
	vtkSmartPointer<vtkPoints>ugpoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray>cells = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkUnstructuredGrid>ug = vtkSmartPointer<vtkUnstructuredGrid>::New();


	for (int z = 1; z < N / n; z++) {

	for (int y = 1; y < N / n; y++) {
	for (int x = 1; x < N / n; x++) {
	if ((x == 1 || x == 51 || y == 1 || y == 51 || z == 1) && x <= 51 && y <= 51 && z <= 51)
	{
	a[z][y][x] = 1;

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
	ugactor->GetProperty()->SetColor(1, 1, 0);


	vtkSmartPointer<vtkRenderer>renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(1, 1, 1);//设置背景颜色
	renderer->AddActor(ugactor);
	vtkSmartPointer<vtkRenderWindow>renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(300, 300);



	//动态显示焊枪
	WeldingTorch w;

	vtkSmartPointer<vtkActor> weldingActor1 = vtkSmartPointer<vtkActor>::New();
	weldingActor1->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(weldingActor1);
	
	vtkSmartPointer<vtkActor> weldingActor2 = vtkSmartPointer<vtkActor>::New();
	weldingActor2->GetProperty()->SetColor(0, 1, 0);
	renderer->AddActor(weldingActor2);
	
	
	
	int z = 2,x=3,y=3,xn=2,yn=2,zn=2;
	while (z <= 2) {
		if (y > 49)y = 49;
		if (y < 3)y = 3;
		while (y >= 3 && y <= 49) {
			if (x > 49)x = 49;
			if (x < 3)x = 3;
			while (x >= 3 && x <= 49) {
				w.setx(x); w.sety(y); w.setz(z);
				w.setlx(0); w.setly(0);
				if (!w.Detection(a))
					w.Dispose(a);
				weldingActor1->SetMapper(w.draw1());
				weldingActor2->SetMapper(w.draw2());
				Sleep(10);

				renderWindow->Render();
				x += xn;
			}
			
			y += yn;
			if (x > 49||x<3)xn *= -1;
		}
		z += zn;
		if (y > 49 || y < 3)yn *= -1;
	}
	renderWindow->Render();



	vtkSmartPointer<vtkRenderWindowInteractor>renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);


	renderWindowInteractor->Start();

	return 0;
}
