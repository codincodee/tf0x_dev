#include "tf0x_common/tf0x_common.h"
#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <QObject>
#include "command_window.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  win.show();
//  CommandWindow cmd_win(&win);
//  cmd_win.show();
  return app.exec();
}
