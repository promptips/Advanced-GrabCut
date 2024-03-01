
#include "form.h"

#include <vtkBorderRepresentation.h>
#include <vtkBorderWidget.h>
#include <vtkCoordinate.h>
#include <vtkImageActor.h>
#include <vtkImageClip.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkJPEGWriter.h>
#include <vtkLookupTable.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <QFileDialog>

#include "ExpectationMaximization/Model.h"
#include "ExpectationMaximization/GaussianND.h"
#include "ExpectationMaximization/vtkExpectationMaximization.h"

#include "ImageGraphCut.h"

Form::Form(QWidget *parent)
{
  setupUi(this);
  connect( this->actionOpen, SIGNAL( triggered() ), this, SLOT(actionOpen_triggered()) );
  connect( this->btnCut, SIGNAL( clicked() ), this, SLOT(btnCut_clicked()) );
