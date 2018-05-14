#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/qt_serial_port.h>
#include <QDebug>

void MainWindow::timerEvent(QTimerEvent *event) {
  if (!sensor_driver_) {
    return;
  }
  std::string buffer;
  tf03_driver::Measurement measurement;
  sensor_driver_->ReadMeasurement(measurement, buffer);

}

void MainWindow::InitializeTimerEvent() {
  ResetSensorDriver();

  startTimer(10);
}

void MainWindow::ResetSensorDriver() {
  qDebug() << __LINE__;
  sensor_serial_.reset(new tf0x_driver::QtSerialPort);
  sensor_serial_->SetPortName(
      ui->SensorSerialPortComboBox->currentText().toStdString());
  sensor_serial_->SetBaudRate(
      ui->SensorSerialBaudRateComboBox->currentText().toInt());
  sensor_serial_->Initialize();

  sensor_driver_.reset(new tf03_driver::Driver);
  sensor_driver_->SetSerialPort(sensor_serial_);
  sensor_driver_->Initialize();
}
