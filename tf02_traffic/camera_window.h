#ifndef CAMERA_WINDOW_H
#define CAMERA_WINDOW_H

#include <QMainWindow>

namespace Ui {
  class CameraWindow;
}

class CameraWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit CameraWindow(QWidget *parent = 0);
  ~CameraWindow();

private:
  Ui::CameraWindow *ui;
};

#endif // CAMERA_WINDOW_H
