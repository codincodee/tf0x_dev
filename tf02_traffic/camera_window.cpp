#include "camera_window.h"
#include "ui_camera_window.h"
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QUrl>
#include <QDateTime>
#include <QMessageBox>

CameraWindow::CameraWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CameraWindow),
  camera_(nullptr),
  camera_view_(nullptr)
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
  if (!camera_view_) {
    return;
  }
  auto info = QCameraInfo(name.toLocal8Bit());
  if (camera_) {
    delete camera_;
  }
  camera_ = new QCamera(info);
  camera_->setViewfinder(camera_view_);
  camera_->start();

  recorder_.reset(new QMediaRecorder(camera_));
  QAudioEncoderSettings settings;
  settings.setCodec("audio/amr");
  settings.setQuality(QMultimedia::HighQuality);
  recorder_->setAudioSettings(settings);
  recorder_->setOutputLocation(
      QUrl::fromLocalFile(QDateTime::currentDateTime().toString() + ".mp4"));
}

bool CameraWindow::StartRecord() {
  if (!recorder_) {
    return false;
  }
  recorder_->record();
  ui->PreviousToolButton->setDisabled(true);
  ui->NextCameraToolButton->setDisabled(true);
  this->setWindowTitle("Camera (recording...)");
  return true;
}

bool CameraWindow::StopRecord() {
  if (!recorder_) {
    return false;
  }
  recorder_->stop();
  ui->PreviousToolButton->setDisabled(false);
  ui->NextCameraToolButton->setDisabled(false);
  this->setWindowTitle("Camera");
  return true;
}

void CameraWindow::OnStartRecord() {
  if (!StartRecord()) {
    QMessageBox::warning(this, "Warning", "Unable to record", QMessageBox::Ok);
  }
}

void CameraWindow::OnStopRecord() {
  if (!StopRecord()) {
    QMessageBox::warning(
        this, "Warning", "Unable to stop recording", QMessageBox::Ok);
  }
}
