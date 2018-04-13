#include "camera_window.h"
#include "ui_camera_window.h"

CameraWindow::CameraWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CameraWindow)
{
  ui->setupUi(this);
}

CameraWindow::~CameraWindow()
{
  delete ui;
}
