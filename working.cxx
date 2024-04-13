
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkBorderRepresentation.h>
#include <vtkBorderWidget.h>
#include <vtkCommand.h>
#include <vtkImageActor.h>
#include <vtkImageClip.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleImage.h>
#include <vtkJPEGReader.h>
#include <vtkMath.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXMLPolyDataReader.h>

#include <string>

class GrabCutInteractorStyle : public vtkInteractorStyleImage
{
public:
  static GrabCutInteractorStyle* New();
  vtkTypeMacro(GrabCutInteractorStyle, vtkInteractorStyleImage);

protected:

};
vtkStandardNewMacro(GrabCutInteractorStyle);

class vtkBorderCallback2 : public vtkCommand
{
public:
  vtkBorderCallback2(){}

  static vtkBorderCallback2 *New()
    {
    return new vtkBorderCallback2;
    }

  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {

    vtkBorderWidget *borderWidget =
      reinterpret_cast<vtkBorderWidget*>(caller);

    // Get the world coordinates of the two corners of the box
    vtkCoordinate* lowerLeftCoordinate =
      static_cast<vtkBorderRepresentation*>
      (borderWidget->GetRepresentation())->GetPositionCoordinate();
    double* lowerLeft =
      lowerLeftCoordinate->GetComputedWorldValue(this->LeftRenderer);
    std::cout << "Lower left coordinate: "
              << lowerLeft[0] << ", "
              << lowerLeft[1] << ", "
              << lowerLeft[2] << std::endl;