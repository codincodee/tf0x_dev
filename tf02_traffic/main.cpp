#include "tf0x_common/tf0x_common.h"
#include "main_window.h"
#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  win.show();
  return app.exec();
}
