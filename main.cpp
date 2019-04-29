#include "test2.h"
#include <QtWidgets/QApplication>

#include<vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)

VTK_MODULE_INIT(vtkInteractionStyle);

VTK_MODULE_INIT(vtkRenderingFreeType);


int main(int argc, char *argv[])
{
	vtkObject::GlobalWarningDisplayOff();//把vtkOutputWindow关闭，在建立QApplication之前关闭
	QApplication a(argc, argv);
	test2 w;
	
	w.show();
	return a.exec();
}
