#ifndef FILEMENUFORM_H
#define FILEMENUFORM_H

#include "ui_form.h"

#include "ImageGraphCut.h"

#include <vtkSmartPointer.h>

#include <vnl/vnl_vector.h>

class vtkBorderWidget;
class vtkImageActor;
class vtkRenderer;
class vtkImageData;
class vtkImageClip;

class Form : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
    Form(QWidget *parent = 0);

public slots:
    void actionOpen_triggered();
    void btnCut_clicked();

protected:
  vtkSmartPointer<ImageGr