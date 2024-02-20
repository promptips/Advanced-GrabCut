#ifndef GrabCutInteractorStyle_h
#define GrabCutInteractorStyle_h

#include <vtkInteractorStyleImage.h>
#include <vtkSmartPointer.h>
//#include "vtkMemberFunctionCommand.h"

class vtkRenderer;
class vtkImageActor;
class vtkImageClip;
class vtkBorderWidget;
class vtkImageData;
class vtkExpectationMaximization;

#include <vnl/vnl_vector.h>
#include <vector>

class GrabCutInteractorStyle : public vtkInteractorStyleImage
{
public:
  static GrabCutInteractorStyl