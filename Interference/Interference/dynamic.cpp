#include <iostream>
#include <stdio.h>
#include <malloc.h>

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkCubeSource.h"

void read_point_file(float p[][3], const char *name1)
{
	FILE *fp = NULL;
	if ((fp = fopen(name1, "r")) == NULL)
	{
		cout << "open file name1 error" << endl;
		return;
	}
	float arr[3];
	int nn = 0;
	fscanf(fp, "%d", &nn);
	int n = 0;
	while (!feof(fp))
	{
		int ret = fscanf(fp, "%f %f %f", &arr[0], &arr[1], &arr[2]);
		if (ret != 3)
			break;
		p[n][0] = arr[0];
		p[n][1] = arr[1];
		p[n][2] = arr[2];
		n++;
	}
	fclose(fp);
}

void read_tri_file(vtkIdType tri[][3], const char *name2)
{
	FILE *fp = NULL;
	if ((fp = fopen(name2, "r")) == NULL)
	{
		cout << "open file name2 error" << endl;
		return;
	}
	int nn = 0;
	fscanf(fp, "%d", &nn);
	int n = 0;
	int num;
	int int_arr[3];
	while (!feof(fp))
	{
		int ret = fscanf(fp, "%d %d %d %d", &num, &int_arr[0], &int_arr[1], &int_arr[2]);
		if (ret != 4)
			break;
		tri[n][0] = int_arr[0];
		tri[n][1] = int_arr[1];
		tri[n][2] = int_arr[2];
		n++;
	}
	fclose(fp);
}

int main1(int argc, char *argv[])
{
	char *file1 = NULL, *file2 = NULL;
	if (argc < 3) {
		file1 = (char*)malloc(20 * sizeof(char));
		file2 = (char*)malloc(20 * sizeof(char));
		file1 = (char *)("a.asc");
		file2 = (char *)("aa.asc");
	}
	else {
		file1 = argv[1];
		file2 = argv[2];
	}
	FILE *fp = NULL;
	int p_num = 0;
	if ((fp = fopen(file1, "r")) == NULL)
	{
		cout << "open file " << argv[1] << " error" << endl;
		return 1;
	}
	fscanf(fp, "%d", &p_num);
	fclose(fp);
	printf("point number is : %d\n", p_num);
	float(*points_arr)[3] = (float((*)[3]))malloc(3 * p_num * sizeof(float));

	int tri_num = 0;
	if ((fp = fopen(file2, "r")) == NULL)
	{
		cout << "open file " << argv[2] << " error" << endl;
		return 1;
	}
	fscanf(fp, "%d", &tri_num);
	fclose(fp);
	vtkIdType(*triangle_arr)[3] = (vtkIdType((*)[3]))malloc(3 * tri_num * sizeof(vtkIdType));
	printf("triangle number is : %d \n", tri_num);

	read_point_file(points_arr, file1);
	read_tri_file(triangle_arr, file2);

	int i;
	vtkPolyData *polydata = vtkPolyData::New();
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *polys = vtkCellArray::New();

	vtkPolyDataMapper *polydataMapper = vtkPolyDataMapper::New();
	polydataMapper->SetInputData(polydata);
	vtkActor *polydataActor = vtkActor::New();
	polydataActor->SetMapper(polydataMapper);
	polydataActor->GetProperty()->SetColor(0.0, 0.0, 1.0);

	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	renderer->AddActor(polydataActor);
	renderer->SetBackground(1, 1, 1);
	renWin->SetSize(400, 400);

	for (i = 0; i<p_num; i++) points->InsertPoint(i, points_arr[i]);
	for (i = 0; i<tri_num; i++) {
		polys->InsertNextCell(3, triangle_arr[i]);
	}
	polydata->SetPoints(points);
	polydata->SetPolys(polys);
	renWin->Render();
	iren->Start();

	for (i = 0; i<p_num; i++) {
		double *pp = polydata->GetPoint(i);
		points = polydata->GetPoints();
		points->SetPoint(i, pp[0] + 10.0, pp[1], pp[2]);
		polydata->SetPoints(points);
		polydataActor->GetProperty()->SetColor(0.0, 0.0, (i*1.0) / (i*1.0 + 0.000001));

		renWin->Render();
	}

	renWin->Render();
	iren->Start();

	polydata->Delete();
	polydataMapper->Delete();
	polydataActor->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();

	return 0;
}