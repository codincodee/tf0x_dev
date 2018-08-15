#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/qt_serial_port.h>
#include <tf0x_drivers/tf03_driver.h>
#include <QDebug>
#include <iostream>
#include <QDir>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  kStartPushButtonStart("Start"),
  kStartPushButtonStop("Stop"),
  stop_signal_(false)
{
  ui->setupUi(this);
  ui->StartPushButton->setText(kStartPushButtonStart);
  startTimer(100);
  on_FreshPushButton_clicked();
}

MainWindow::~MainWindow()
{
  stop_signal_ = true;
  if (thread_.joinable()) {
    thread_.join();
  }
  delete ui;
}

void MainWindow::on_StartPushButton_clicked()
{
  auto button = ui->StartPushButton;
  if (button->text() == kStartPushButtonStart) {
    if (OnStart()) {
      button->setText(kStartPushButtonStop);
    }
  } else {
    if (OnStop()) {
      button->setText(kStartPushButtonStart);
    }
  }
}

bool MainWindow::OnStart() {
  stop_signal_ = false;
  std::vector<std::string> port_names;
  auto serial_port =
      ui->SerialPortAlphaComboBox->currentText().toStdString();
  if (serial_port.empty()) {
    return false;
  }
  port_names.push_back(serial_port);
  serial_port = ui->SerialPortBetaComboBox->currentText().toStdString();
  if (!serial_port.empty()) {
    port_names.push_back(serial_port);
  }

  serial_ports.clear();
  drivers.clear();
  for (auto serial : port_names) {
    std::shared_ptr<tf0x_driver::QtSerialPort> serial_port(
        new tf0x_driver::QtSerialPort);
    serial_ports.push_back(serial_port);
    serial_port->SetPortName(serial);
    serial_port->SetBaudRate(115200);
    serial_port->Initialize();

    std::shared_ptr<tf03_driver::Driver> driver(new tf03_driver::Driver);
    drivers.push_back(driver);
    driver->SetSerialPort(serial_port);
    driver->Initialize();
  }
  thread_ =
      std::thread(std::bind(&MainWindow::ReadingThread, this, port_names));
  return true;
}

bool MainWindow::OnStop() {
  stop_signal_ = true;
  if (thread_.joinable()) {
    thread_.join();
  }
  SaveReadings();
  return true;
}

void MainWindow::timerEvent(QTimerEvent *event) {
  std::vector<tf03_driver::Measurement> measurements;
  latest_measurements_mutex_.lock();
  measurements = latest_measurements_;
  latest_measurements_mutex_.unlock();
  // qDebug() << measurements.size();
  switch(measurements.size()) {
  case 2:
    ui->DistanceBetaLabel->setText(QString::number(measurements[1].dist1));
  case 1:
    ui->DistanceLabelAlpha->setText(QString::number(measurements[0].dist1));
  }
}

void MainWindow::on_FreshPushButton_clicked()
{
  auto ports = tf0x_driver::QtSerialPort::ScanAllPorts();
  ui->SerialPortAlphaComboBox->clear();
  ui->SerialPortBetaComboBox->clear();
  for (auto port : ports) {
    ui->SerialPortAlphaComboBox->addItem(QString::fromStdString(port));
    ui->SerialPortBetaComboBox->addItem(QString::fromStdString(port));
  }
  ui->SerialPortAlphaComboBox->addItem("");
  ui->SerialPortBetaComboBox->addItem("");
}

void MainWindow::ReadingThread(std::vector<std::string> serials) {
  auto device_cnt = serials.size();
  readings_.clear();
  for (int i = 0; i < device_cnt; ++i) {
    readings_.push_back(std::list<tf03_driver::Measurement>());
  }

  while (!stop_signal_) {
    std::vector<tf03_driver::Measurement> latest_measurements;
    for (int i = 0; i < device_cnt; ++i) {
      std::string buffer;
      std::vector<tf03_driver::Measurement> measurements;
      measurements = drivers[i]->ReadMeasurements(buffer);
      // qDebug() << measurements.size();
      for (auto& measurement : measurements) {
        readings_[i].push_back(measurement);
      }
      if (!readings_[i].empty()) {
        latest_measurements.push_back(*readings_[i].rbegin());
      }
    }
    latest_measurements_mutex_.lock();
    latest_measurements_ = latest_measurements;
    latest_measurements_mutex_.unlock();
  }
}

void MainWindow::SaveReadings() {
  const QString data_dir = "data";
  if (!QDir(data_dir).exists()) {
    QDir dir;
    dir.mkdir(data_dir);
  }
  for (int i = 0; i < readings_.size(); ++i) {
    QFile file(data_dir + "/sensor_" + QString::number(i + 1) + ".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << "# Raw-Distance-1(cm) Raw-Distance-2(cm) Raw-Distance-3(cm) Distance-1(cm) Distance-2(cm) Distance-3(cm) APD-Voltage(V) Laser-Voltage(V) Algorithm Temperature(C)\n";
    for (auto& entry : readings_[i]) {
      stream
          << entry.raw_dist1 << " "
          << entry.raw_dist2 << " "
          << entry.raw_dist3 << " "
          << entry.dist1 << " "
          << (short)entry.dist2 << " "
          << entry.dist3 << " "
          << entry.apd << " "
          << entry.volt << " "
          << entry.algorithm << " "
          << entry.temp << "\n";
    }
  }
}
