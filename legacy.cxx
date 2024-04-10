
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkImageActor.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleRubberBand2D.h>

void SelectionChangedCallbackFunction ( vtkObject* caller, long unsigned int eventId, void* clientData, void* callData );

int main(int argc, char* argv[])
{
  // Parse input arguments
  if ( argc != 2 )
  {
    std::cout << "Required parameters: Filename" << std::endl;
    exit(-1);
  }

  vtkstd::string inputFilename = argv[1];

  // Read the image
  vtkSmartPointer<vtkJPEGReader> reader =
    vtkSmartPointer<vtkJPEGReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  // Create an actor
  vtkSmartPointer<vtkImageActor> actor =
    vtkSmartPointer<vtkImageActor>::New();
  actor->SetInput(reader->GetOutput());

  // Setup the SelectionChangedEvent callback
  vtkSmartPointer<vtkCallbackCommand> selectionChangedCallback =
    vtkSmartPointer<vtkCallbackCommand>::New();
  selectionChangedCallback->SetCallback(SelectionChangedCallbackFunction);

   // Define viewport ranges
  // (xmin, ymin, xmax, ymax)
  double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
  double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

  // Setup renderer
  vtkSmartPointer<vtkRenderer> leftRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  leftRenderer->AddActor(actor);
  leftRenderer->ResetCamera();
  leftRenderer->SetViewport(leftViewport);

  // Setup renderer
  vtkSmartPointer<vtkRenderer> rightRenderer =
    vtkSmartPointer<vtkRenderer>::New();
  //rightRenderer->AddActor(actor);
  rightRenderer->ResetCamera();
  rightRenderer->SetViewport(rightViewport);

  // Setup render window
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(leftRenderer);
  renderWindow->AddRenderer(rightRenderer);
  renderWindow->SetSize(800,400);
