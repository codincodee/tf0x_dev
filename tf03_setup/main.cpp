#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <thread>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  win.show();

  std::thread thread([argc, argv](){
    int ac = argc;
    QApplication app(ac, argv);
    MainWindow win;
    win.show();
    app.exec();
  });
  return app.exec();
}
