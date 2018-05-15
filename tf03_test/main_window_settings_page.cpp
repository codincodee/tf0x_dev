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

void MainWindow::FillSerialPortComboBox(QComboBox &combo_box) {
  auto ports = tf0x_driver::QtSerialPort::ScanAllPorts();
  QStringList ports_sl;
  for (auto& port : ports) {
    ports_sl.push_back(QString::fromStdString(port));
  }
  QtHelpers::UpdateComboBoxWithoutCurrentChanged(combo_box, ports_sl);
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

QString gSettingsPreviousLogPath;
QString gSettingsPreviousSensorSerialPort;
QString gSettingsPreviousSensorBaudRate;

void MainWindow::EnteringSettingsPage() {
  gSettingsPreviousLogPath = ui->LogPathLineEdit->text();
  gSettingsPreviousSensorSerialPort =
      ui->SensorSerialPortComboBox->currentText();
  gSettingsPreviousSensorBaudRate =
      ui->SensorSerialBaudRateComboBox->currentText();

  FillSerialPortComboBox(*ui->SensorSerialPortComboBox);
}

void MainWindow::LeavingSettingsPage() {
  if (ui->SensorSerialBaudRateComboBox->currentText() !=
          gSettingsPreviousSensorBaudRate ||
      ui->SensorSerialPortComboBox->currentText() !=
          gSettingsPreviousSensorSerialPort) {
    ResetSensorDriver();
  }
}

void MainWindow::on_SettingsRefreshPushButton_clicked()
{
  FillSerialPortComboBox(*ui->SensorSerialPortComboBox);
}
