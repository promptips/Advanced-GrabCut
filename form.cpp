
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

  // Setup renderers
  this->LeftRenderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkInteractorStyleImage> interactorStyleImage =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  this->qvtkWidgetLeft->GetInteractor()->SetInteractorStyle(interactorStyleImage);
  this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(this->LeftRenderer);

  this->RightRenderer = vtkSmartPointer<vtkRenderer>::New();
  this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(this->RightRenderer);

  // Instantiations
  this->GraphCut = vtkSmartPointer<ImageGraphCut>::New();
  this->OriginalImageActor = vtkSmartPointer<vtkImageActor>::New();
  this->OriginalImage = vtkSmartPointer<vtkImageData>::New();
  this->BorderWidget = vtkSmartPointer<vtkBorderWidget>::New();

  // Setup ClipFilter for cropping
  this->ClipFilter = vtkSmartPointer<vtkImageClip>::New();
  this->ClipFilter->SetInputData(this->OriginalImage);
  this->ClippedActor = vtkSmartPointer<vtkImageActor>::New();

  // Setup BorderWidget (the rectangle used to crop the left (original) image
  this->BorderWidget->SetInteractor(this->qvtkWidgetLeft->GetInteractor());
  this->BorderWidget->On();
  this->BorderWidget->AddObserver(vtkCommand::InteractionEvent, this, &Form::CatchWidgetEvent);

}

void Form::actionOpen_triggered()
{
  // Get a filename to open
  QString filename = QFileDialog::getOpenFileName(this,
     tr("Open Image"), "/media/portable/Projects/src/GrabCut/data", tr("Image Files (*.png *.jpg *.bmp)"));

  if(filename.isEmpty())
    {
    std::cout << "Cancelled." << std::endl;
    return;
    }
