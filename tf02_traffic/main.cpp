#include "tf0x_common/tf0x_common.h"
#include "main_window.h"
#include <QApplication>
#include "camera_window.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  win.show();
  CameraWindow cam;
  cam.show();
  return app.exec();
}
