#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/abstract_serial_port.h>
#include <tf0x_drivers/qt_serial_port.h>
#include <tf0x_common/qt_helpers.h>
#include <QSettings>
#include <QDebug>

void MainWindow::FillComboBoxWithBaudRate(QComboBox& combo_box) {
  auto rates = tf0x_driver::AbstractSerialPort::BaudRateLists();
  for (auto& rate : rates) {
    combo_box.addItem(QString::number(rate));
  }
  combo_box.setCurrentText(QString::number(115200));
}

bool MainWindow::SetSensorSerialPort(tf0x_driver::AbstractSerialPort &port) {
  port.SetPortName(ui->SensorSerialPortComboBox->currentText().toStdString());
  port.SetBaudRate(ui->SensorSerialBaudRateComboBox->currentText().toInt());
  return true;
}

void MainWindow::InitializeSettingsPage() {
  FillComboBoxWithBaudRate(*ui->SensorSerialBaudRateComboBox);
  FillSerialPortComboBox(*ui->SensorSerialPortComboBox);
  ui->LogPathLineEdit->setReadOnly(true);
  LoadSettingsFromConfigFile();
}

void MainWindow::FillSerialPortComboBox(QComboBox& combo_box) {
  qDebug() << __LINE__;
  auto ports = tf0x_driver::QtSerialPort::ScanAllPorts();
  qDebug() << __LINE__;
  QStringList ports_sl;
  qDebug() << __LINE__;
  for (auto& port : ports) {
    ports_sl.push_back(QString::fromStdString(port));
  }
  qDebug() << __LINE__;
  QtHelpers::UpdateComboBoxWithoutCurrentChanged(
      combo_box, ports_sl);
  qDebug() << __LINE__;
}

void MainWindow::on_SensorSerialPortComboBox_currentTextChanged(const QString &arg1)
{
  ResetSensorDriver();
}

void MainWindow::on_SensorSerialBaudRateComboBox_currentTextChanged(const QString &arg1)
{
  ResetSensorDriver();
}

void MainWindow::on_LogPathPushButton_clicked()
{
  auto folder = QtHelpers::SelectFolder(this);
  if (!folder.isEmpty()) {
    ui->LogPathLineEdit->setText(folder);
  }
}

const QString kSettingsKeyLogPath = "log_path";

void MainWindow::LoadSettingsFromConfigFile() {
  QSettings settings(ConfigFilePath(), QSettings::IniFormat);
  ui->LogPathLineEdit->setText(
      settings.value(
          kSettingsKeyLogPath,
          QtHelpers::SystemDocumentFolderPath()).toString());
}

void MainWindow::SaveSettingsToConfigFile() {
  QSettings settings(ConfigFilePath(), QSettings::IniFormat);
  settings.setValue(kSettingsKeyLogPath, ui->LogPathLineEdit->text());
}

QString MainWindow::ConfigFilePath() {
  return QtHelpers::SystemDocumentFolderPath() + "/tf03_test.config.ini";
}

void MainWindow::SettingsPageClicked() {
  FillSerialPortComboBox(*ui->SensorSerialPortComboBox);
}
