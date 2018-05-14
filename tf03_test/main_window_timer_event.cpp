#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/qt_serial_port.h>

void MainWindow::timerEvent(QTimerEvent *event) {

}

void MainWindow::InitializeTimerEvent() {
  std::shared_ptr<tf0x_driver::QtSerialPort> serial;
  driver_.reset(new tf03_driver::Driver);

  startTimer(10);
}
