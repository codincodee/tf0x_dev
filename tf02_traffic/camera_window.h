#ifndef CAMERA_WINDOW_H
#define CAMERA_WINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>

namespace Ui {
  class CameraWindow;
}

class CameraWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit CameraWindow(QWidget *parent = 0);
  ~CameraWindow();

protected:
  void closeEvent(QCloseEvent* event);

private slots:
  void on_NextCameraToolButton_clicked();

  void on_PreviousToolButton_clicked();

private:
  void UseCamera(const QString& name);
  Ui::CameraWindow *ui;
  QCameraViewfinder* camera_view_;
  QCamera* camera_;
  QString current_camera_name_;
};

#endif // CAMERA_WINDOW_H
