#include "tf0x_common/tf0x_common.h"
#include "main_window.h"
#include <QApplication>
#include "camera_window.h"
#include <QDebug>
#include <QObject>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  CameraWindow cam;

//  QObject::connect(&win, SIGNAL(StartRecord()), &cam, SLOT(OnStartRecord()));
//  QObject::connect(&win, SIGNAL(StopRecord()), &cam, SLOT(OnStopRecord()));

  win.show();
  cam.show();
  return app.exec();
}
