#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/qt_serial_port.h>
#include <QDebug>

void MainWindow::HandleSensorTimerEvent() {
  if (!sensor_driver_) {
    return;
  }
  std::string buffer;
  tf03_driver::Measurement measurement;
  // sensor_driver_->ReadMeasurement(measurement, buffer);
  // sensor_serial_->ReadBuffer(buffer);
  if (!sensor_driver_->ReadMeasurement(measurement, buffer)) {
    HandleIncomingStream(buffer);
    return;
  }
  // qDebug() << measurement.dist1;

  HandleIncomingMeasurement(measurement);
//  qDebug() << measurement.dists[0];
//  measurement_cache_.push_back(measurement);
//  if (measurement_cache_.size() > 10) {
//    for (auto& mea : measurement_cache_) {
//      HandleIncomingMeasurement(mea);
//    }
//    measurement_cache_.clear();
//  }

  static QElapsedTimer timer;
  static int cnt = 0;
  ++cnt;
  if (timer.elapsed() > 1000) {
    this->setWindowTitle("TF03 Test (" + QString::number(cnt) + ")");
    timer.restart();
    cnt = 0;
  }
  last_measurement_ = measurement;
}

void MainWindow::HandleCartTimerEvent() {
  if (!cart_driver_) {
    return;
  }
  cart_driver::Instruction instruction;
  int repetition;
  if (!cart_driver_->ReadInstruction(instruction, repetition)) {
    return;
  }
  HandleCartInstruction(instruction, repetition);
}

void MainWindow::timerEvent(QTimerEvent *event) {
  HandleSensorTimerEvent();
  HandleCartTimerEvent();
}

void MainWindow::InitializeTimerEvent() {
  ResetSensorDriver();
  ResetCartDriver();

  startTimer(0);
}

void MainWindow::ResetSensorDriver() {
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

void MainWindow::ResetCartDriver() {
  cart_serial_.reset(new tf0x_driver::QtSerialPort);
  cart_serial_->SetPortName(
      ui->CartSerialPortComboBox->currentText().toStdString());
  cart_serial_->SetBaudRate(
      ui->CartSerialBaudRateComboBox->currentText().toInt());
//  cart_serial_->SetBaudRate(460800);
  cart_serial_->Initialize();

  cart_driver_.reset(new cart_driver::Driver);
  cart_driver_->SetSerialPort(cart_serial_);
  cart_driver_->Initialize();
}
