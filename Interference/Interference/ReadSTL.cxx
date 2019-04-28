#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main1( int argc, char *argv[] )
{

  if ( argc != 3 )
  {
    cout << "Required parameters: Filename" << endl;
    return EXIT_FAILURE;
  }
  argv[1] = "mujuxiufu.stl";
  argv[2] = "muju.stl";
  std::string inputFilename = argv[2];

  vtkSmartPointer<vtkSTLReader> reader =
    vtkSmartPointer<vtkSTLReader>::New();
  //reader->SetFileName(inputFilename.c_str());
  reader->SetFileName("D:\\VS2015-practice\\Interference\\Interference\\muju-new.stl");
  reader->Update();
  vtkSmartPointer<vtkSTLReader> reader1 =
	  vtkSmartPointer<vtkSTLReader>::New();
  //reader->SetFileName(inputFilename.c_str());
  reader1->SetFileName("D:\\VS2015-practice\\Interference\\Interference\\mujuxiufu-new.stl");
  reader1->Update();

  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());

  vtkSmartPointer<vtkPolyDataMapper> mapper1 =
	  vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper1->SetInputConnection(reader1->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkActor> actor1 =
	  vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor1->SetMapper(mapper1);
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->AddActor(actor1);
  renderer->SetBackground(.3, .6, .3); // Background color green

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
