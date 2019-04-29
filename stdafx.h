// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO:  在此处引用程序需要的其他头文件
//有空检查一下VTK的头文件有没有重复的
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkCutter.h"
#include "vtkPoints.h"
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
#include "vtkAnimationScene.h"
#include "vtkAnimationCue.h"
#include "vtkAxesActor.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkRegularPolygonSource.h"
#include "vtkAppendPolyData.h"
#include "vtkArcSource.h"
#include "vtkTubeFilter.h"
#include "vtkCellType.h"
#include "vtkProperty.h"
#include "vtkCylinder.h"
#include "vtkImplicitBoolean.h"
#include "vtkSampleFunction.h"
#include "vtkContourFilter.h"
#include "vtkCommand.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkVolumeProperty.h"
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
//#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
//#include <vtkVolumeRayCastMapper.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPNGReader.h>
#include <vtkJPEGReader.h>
#include <vtkArrowSource.h>
#include <vtkConeSource.h>
#include<vtkSTLReader.h>
#include "vtkTexture.h"

#include "3DRepresentation.h"
#include "TEACDef.h"
#include"SolidDef.h"
#include <math.h>
#include <stdlib.h>
#include<stack>
#include <vector>