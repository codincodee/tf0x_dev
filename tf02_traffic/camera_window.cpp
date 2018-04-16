#include "camera_window.h"
#include "ui_camera_window.h"
#include <QCameraViewfinder>
#include <QCameraInfo>

CameraWindow::CameraWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CameraWindow)
{
  ui->setupUi(this);
  this->resize(1000, 800);
  camera_view_ = new QCameraViewfinder;
  ui->ImageVerticalLayout->addWidget(camera_view_);
  current_camera_name_ = QCameraInfo::defaultCamera().deviceName();
  UseCamera(current_camera_name_);
  this->setWindowTitle("Camera");
}

void CameraWindow::closeEvent(QCloseEvent *event) {
  if (camera_) {
    camera_->stop();
  }
  return QMainWindow::closeEvent(event);
}

CameraWindow::~CameraWindow()
{
  if (camera_view_) {
    delete camera_view_;
  }
  if (camera_) {
    delete camera_;
  }
  delete ui;
}

void CameraWindow::on_NextCameraToolButton_clicked()
{
  auto cameras = QCameraInfo::availableCameras();
  QString camera;
  for (int i = 0; i < cameras.size(); ++i) {
    if (cameras[i].deviceName() == current_camera_name_) {
      if (i == (cameras.size() - 1)) {
        if (cameras.size() > 1) {
          camera = cameras[0].deviceName();
          UseCamera(camera);
        }
      } else {
        camera = cameras[i + 1].deviceName();
        UseCamera(camera);
      }
      break;
    }
  }
  if (camera.isEmpty()) {
    if (!cameras.isEmpty()) {
      camera = cameras[0].deviceName();
    }
  }
  current_camera_name_ = camera;
}

void CameraWindow::on_PreviousToolButton_clicked()
{
  auto cameras = QCameraInfo::availableCameras();
  QString camera;
  for (int i = 0; i < cameras.size(); ++i) {
    if (cameras[i].deviceName() == current_camera_name_) {
      if (i == 0) {
        if (cameras.size() > 1) {
          camera = cameras[cameras.size() - 1].deviceName();
          UseCamera(camera);
        }
      } else {
        camera = cameras[i - 1].deviceName();
        UseCamera(camera);
        break;
      }
      break;
    }
  }
  if (camera.isEmpty()) {
    if (!cameras.isEmpty()) {
      camera = cameras[0].deviceName();
    }
  }
  current_camera_name_ = camera;
}

void CameraWindow::UseCamera(const QString &name) {
  auto info = QCameraInfo(name.toLocal8Bit());
  if (camera_) {
    delete camera_;
  }
  camera_ = new QCamera(info);
  camera_->setViewfinder(camera_view_);
  camera_->start();
}
