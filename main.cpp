#include "test2.h"
#include <QtWidgets/QApplication>

#include<vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)

VTK_MODULE_INIT(vtkInteractionStyle);

VTK_MODULE_INIT(vtkRenderingFreeType);


int main(int argc, char *argv[])
{
	vtkObject::GlobalWarningDisplayOff();//��vtkOutputWindow�رգ��ڽ���QApplication֮ǰ�ر�
	QApplication a(argc, argv);
	test2 w;
	
	w.show();
	return a.exec();
}
