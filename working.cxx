
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

    vtkCoordinate* upperRightCoordinate =
      static_cast<vtkBorderRepresentation*>
      (borderWidget->GetRepresentation())->GetPosition2Coordinate();
    double* upperRight =
      upperRightCoordinate->GetComputedWorldValue(this->LeftRenderer);
    std::cout << "Upper right coordinate: "
              << upperRight[0] << ", "
              << upperRight[1] << ", "
              << upperRight[2] << std::endl;

    double* bounds = this->ImageActor->GetBounds();
    double xmin = bounds[0];
    double xmax = bounds[1];
    double ymin = bounds[2];
    double ymax = bounds[3];

    if( (lowerLeft[0] > xmin) &&
        (upperRight[0] < xmax) &&
        (lowerLeft[1] > ymin) &&
        (upperRight[1] < ymax) )
      {
      this->ClipFilter->SetOutputWholeExtent(
        vtkMath::Round(lowerLeft[0]),
        vtkMath::Round(upperRight[0]),
        vtkMath::Round(lowerLeft[1]),
        vtkMath::Round(upperRight[1]), 0, 1);
      }
    else
      {
      std::cout << "box is NOT inside image" << std::endl;
      }
    }

  void SetLeftRenderer(vtkSmartPointer<vtkRenderer> renderer)
    {this->LeftRenderer = renderer;}