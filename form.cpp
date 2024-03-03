
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

  std::cout << "Got filename: " << filename.toStdString() << std::endl;

  // Read file
  vtkSmartPointer<vtkImageReader2Factory> readerFactory =
    vtkSmartPointer<vtkImageReader2Factory>::New();
  vtkImageReader2* imageReader = readerFactory->CreateImageReader2(filename.toStdString().c_str());
  imageReader->SetFileName(filename.toStdString().c_str());
  imageReader->Update();

  this->OriginalImage->ShallowCopy(imageReader->GetOutput());

  this->OriginalImageActor->SetInputData(this->OriginalImage);

  imageReader->Delete();

  this->LeftRenderer->AddActor(OriginalImageActor);
  this->LeftRenderer->ResetCamera();
}

void Form::Refresh()
{
  this->LeftRenderer->Render();
  this->RightRenderer->Render();
  this->qvtkWidgetRight->GetRenderWindow()->Render();
  this->qvtkWidgetLeft->GetRenderWindow()->Render();
}

#if 0

  vtkSmartPointer<vtkImageClip> imageClip =
    vtkSmartPointer<vtkImageClip>::New();
  imageClip->SetInputConnection(jPEGReader->GetOutputPort());
  imageClip->SetOutputWholeExtent(jPEGReader->GetOutput()->GetWholeExtent());
  imageClip->ClipDataOn();

  vtkSmartPointer<vtkImageActor> clipActor =
    vtkSmartPointer<vtkImageActor>::New();
  clipActor->SetInput(imageClip->GetOutput());
  rightRenderer->AddActor(clipActor);

  vtkSmartPointer<GrabCutInteractorStyle> style =
    vtkSmartPointer<GrabCutInteractorStyle>::New();
  interactor->SetInteractorStyle(style);
  style->SetLeftRenderer(leftRenderer);
  style->SetRightRenderer(rightRenderer);
  style->SetImageActor(imageActor);
  style->SetClipFilter(imageClip);
  style->SetInputImage(jPEGReader->GetOutput());

  interactor->SetRenderWindow(renderWindow);
  renderWindow->Render();

  interactor->Start();
#endif

void Form::btnCut_clicked()
{
  // Setup the mask
  int extent[6];
  this->OriginalImage->GetExtent(extent);
  //PrintExtent("extent", extent);

  this->AlphaMask->SetExtent(extent);
  this->AlphaMask->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
  
  int clippedExtent[6];
  ClipFilter->GetOutput()->GetExtent(clippedExtent);
  //PrintExtent("clippedExtent", clippedExtent);

  // Initialize the mask (everything background)
  for(int y = extent[2]; y <= extent[3]; y++)
    {
    for(int x = extent[0]; x <= extent[1]; x++)
      {
      unsigned char* pixel = static_cast<unsigned char*>(this->AlphaMask->GetScalarPointer(x,y,0));
      pixel[0] = ImageGraphCut::ALWAYSSINK;
      }
    }

  // Mask the foreground
  for(int y = clippedExtent[2]; y <= clippedExtent[3]; y++)
    {
    for(int x = clippedExtent[0]; x <= clippedExtent[1]; x++)
      {
      unsigned char* pixel = static_cast<unsigned char*>(this->AlphaMask->GetScalarPointer(x,y,0));
      pixel[0] = ImageGraphCut::SOURCE;