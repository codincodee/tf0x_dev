#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/abstract_serial_port.h>
#include <tf0x_drivers/qt_serial_port.h>
#include <tf0x_common/qt_helpers.h>

void MainWindow::FillComboBoxWithBaudRate(QComboBox *combo_box) {
  auto rates = tf0x_driver::AbstractSerialPort::BaudRateLists();
  for (auto& rate : rates) {
    combo_box->addItem(QString::number(rate));
  }
  combo_box->setCurrentText(QString::number(115200));
}

bool MainWindow::SetSensorSerialPort(tf0x_driver::AbstractSerialPort &port) {
  port.SetPortName(ui->SensorSerialPortComboBox->currentText().toStdString());
  port.SetBaudRate(ui->SensorSerialBaudRateComboBox->currentText().toInt());
  return true;
}

void MainWindow::SetSettingsPage() {
  FillComboBoxWithBaudRate(ui->SensorSerialBaudRateComboBox);
  FillSerialPortComboBox(ui->SensorSerialPortComboBox);
  ui->LogPathLineEdit->setReadOnly(true);
}

void MainWindow::FillSerialPortComboBox(QComboBox *combo_box) {
  auto ports = tf0x_driver::QtSerialPort::ScanAllPorts();
  for (auto& port : ports) {
    combo_box->addItem(QString::fromStdString(port));
  }
}

void MainWindow::on_LogPathPushButton_clicked()
{
  ui->LogPathLineEdit->setText(QtHelpers::SelectFolder(this));
}

