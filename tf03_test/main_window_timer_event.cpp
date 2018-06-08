#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/qt_serial_port.h>
#include <QDebug>
#include <QThread>

void MainWindow::timerEvent(QTimerEvent *event) {
  HandleSensorTimerEvent();
  HandleCartTimerEvent();
  HandleSerialDetectionEvent();
}

void MainWindow::InitializeTimerEvent() {
  sensor_thread_exit_signal_ = false;
  sensor_logging_ = false;
  sensor_frequency_ = 0.0f;
  cart_thread_exit_signal_ = false;
  cart_logging_ = false;

  ResetSensorDriver();
  ResetCartDriver();

  startTimer(50);
}

void MainWindow::HandleSerialDetectionEvent() {
  std::string current_port;
  if (sensor_serial_) {
    current_port = sensor_serial_->GetPortName();
  }
  auto ports = tf0x_driver::QtSerialPort::ScanAllPorts();
  bool port_still_on = false;
  for (auto& p : ports) {
    if (p == current_port) {
      port_still_on = true;
    }
  }
  if (!port_still_on) {
    sensor_serial_pending_reconnect_ = true;
  } else {
    if (sensor_serial_pending_reconnect_) {
      ResetSensorDriver();
      sensor_serial_pending_reconnect_ = false;
    }
  }
}

void MainWindow::ResetSensorDriver() {
  main_chart_->Clear();
  sensor_driver_mutex_.lock();
  sensor_serial_.reset(new tf0x_driver::QtSerialPort);
  sensor_serial_->SetPortName(
      ui->SensorSerialPortComboBox->currentText().toStdString());
  sensor_serial_->SetBaudRate(
      ui->SensorSerialBaudRateComboBox->currentText().toInt());
  sensor_serial_->Initialize();

  sensor_driver_.reset(new tf03_driver::Driver);
  sensor_driver_->SetSerialPort(sensor_serial_);
  sensor_driver_->Initialize();
  sensor_driver_mutex_.unlock();
}

void MainWindow::ResetCartDriver() {
  cart_driver_mutex_.lock();
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
  cart_driver_mutex_.unlock();
}

////////////////////// Sensor Thread ///////////////////////////

constexpr int kSensorReadingsSizeLimits = 4000;

void MainWindow::SensorThread() {
  while (!sensor_thread_exit_signal_) {
    // QThread::msleep(1);
    sensor_driver_mutex_.lock();
    if (!sensor_driver_) {
      sensor_driver_mutex_.unlock();
      continue;
    }
    std::string buffer;
    std::vector<tf03_driver::Measurement> measurements;
    measurements = sensor_driver_->ReadMeasurements(buffer);
    sensor_driver_mutex_.unlock();

    if (measurements.empty()) {
      continue;
    }

    static QElapsedTimer timer;
    static int cnt = 0;
    auto elapsed = timer.elapsed();
    if (elapsed < 1000) {
      cnt += measurements.size();
    } else {
      sensor_frequency_ = cnt * 1000.0f / elapsed;
      timer.restart();
      cnt = 0;
    }

//    sensor_readings_mutex_.lock();
//    if (sensor_readings_.size() > kSensorReadingsSizeLimits) {
//      sensor_readings_.clear();
//    }
//    sensor_readings_.push_back(measurement);
//    sensor_readings_mutex_.unlock();

    if (sensor_logging_) {
      sensor_log_mutex_.lock();
      for (auto& measurement : measurements) {
        sensor_log_.push_back(measurement);
      }
      sensor_log_mutex_.unlock();
    }

    sensor_last_measurement_mutex_.lock();
    sensor_last_measurement_ = *(measurements.end() - 1);
    sensor_last_measurement_mutex_.unlock();
  }
}

////////////////////// Sensor Thread ///////////////////////////

void MainWindow::CartThread() {
  while (!cart_thread_exit_signal_) {
    cart_driver_mutex_.lock();
    if (!cart_driver_) {
      cart_driver_mutex_.unlock();
      continue;
    }
    cart_driver::Instruction instruction;
    int repeat;
    bool read_success = cart_driver_->ReadInstruction(instruction, repeat);
    cart_driver_mutex_.unlock();

    if (!read_success) {
      continue;
    }

//    static QElapsedTimer timer;
//    static int cnt = 0;
//    if (timer.elapsed() < 1000) {
//      ++cnt;
//    } else {
//      qDebug() << cnt * 1000 / timer.elapsed();
//      timer.restart();
//      cnt = 0;
//    }

    switch (instruction.type) {
    case cart_driver::Instruction::Type::read_sensor:
      if (cart_logging_) {
        cart_last_measurement_mutex_.lock();
        auto cart_last = cart_last_measurement_;
        cart_last_measurement_mutex_.unlock();

        sensor_last_measurement_mutex_.lock();
        auto sensor_last = sensor_last_measurement_;
        sensor_last_measurement_mutex_.unlock();

        for (int i = 0; i < repeat; ++i) {
          ++cart_last.id;
          cart_last.pos += cart_driver_->StopInterval() * 100.0f;
          cart_last.measurement = sensor_last;

          cart_readings_mutex_.lock();
          cart_readings_.push_back(cart_last);
          cart_readings_mutex_.unlock();
        }
        cart_last_measurement_mutex_.lock();
        cart_last_measurement_ = cart_last;
        cart_last_measurement_mutex_.unlock();
      }
      break;
    case cart_driver::Instruction::Type::reach_start_point:
      cart_logging_ = true;
      break;
    case cart_driver::Instruction::Type::reach_end_point:
      cart_logging_ = false;
      break;
    }
  }
}
