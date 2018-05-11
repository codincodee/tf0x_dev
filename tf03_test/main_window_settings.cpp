#include "main_window.h"
#include "ui_main_window.h"

bool MainWindow::SetSensorSerialPort(tf0x_driver::AbstractSerialPort &port) {
  port.SetPortName(ui->SensorSerialPortComboBox->currentText().toStdString());
  port.SetBaudRate(ui->SensorSerialBaudRateComboBox->currentText().toInt());
  return true;
}
