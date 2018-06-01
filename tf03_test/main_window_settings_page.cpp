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
  FillComboBoxWithBaudRate(*ui->CartSerialBaudRateComboBox);
  FillSerialPortComboBox(*ui->CartSerialPortComboBox);
  ui->LogPathLineEdit->setReadOnly(true);
  LoadSettingsFromConfigFile();
  auto validator = new QDoubleValidator(0, 200, 2, this);
  ui->SettingsCartDistanceLineEdit->setValidator(validator);
  ui->CartPageDistanceLineEdit->setValidator(validator);
  ui->SettingsCartIntervalLineEdit->setValidator(validator);
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
const QString kSettingsCartDistance = "cart_distance";
const QString kSettingsCartInterval = "cart_interval";
const QString kSettingsCartBaudRate = "cart_baud_rate";
const QString kSettingsSensorBaudRate = "sensor_baud_rate";

void MainWindow::LoadSettingsFromConfigFile() {
  QSettings settings(ConfigFilePath(), QSettings::IniFormat);
  ui->LogPathLineEdit->setText(
      settings.value(
          kSettingsKeyLogPath,
          QtHelpers::SystemDocumentFolderPath()).toString());
  ui->SettingsCartDistanceLineEdit->setText(
      settings.value(kSettingsCartDistance, 150).toString());
  ui->CartPageDistanceLineEdit->setText(
      ui->SettingsCartDistanceLineEdit->text());
  ui->SettingsCartIntervalLineEdit->setText(
      settings.value(kSettingsCartInterval, 0.01).toString());
  ui->CartSerialBaudRateComboBox->setCurrentText(
      settings.value(kSettingsCartBaudRate, 460800).toString());
  ui->SensorSerialBaudRateComboBox->setCurrentText(
      settings.value(kSettingsSensorBaudRate, 115200).toString());
}

void MainWindow::SaveSettingsToConfigFile() {
  QSettings settings(ConfigFilePath(), QSettings::IniFormat);
  settings.setValue(kSettingsKeyLogPath, ui->LogPathLineEdit->text());
  settings.setValue(
      kSettingsCartDistance, ui->SettingsCartDistanceLineEdit->text());
  settings.setValue(
      kSettingsCartInterval, ui->SettingsCartIntervalLineEdit->text());
  settings.setValue(
      kSettingsCartBaudRate, ui->CartSerialBaudRateComboBox->currentText());
  settings.setValue(
      kSettingsSensorBaudRate, ui->SensorSerialBaudRateComboBox->currentText());
}

QString MainWindow::ConfigFilePath() {
  return QtHelpers::SystemDocumentFolderPath() + "/tf03_test.config.ini";
}

QString gSettingsPreviousLogPath;
QString gSettingsPreviousSensorSerialPort;
QString gSettingsPreviousSensorBaudRate;
QString gSettingsPreviousCartSerialPort;
QString gSettingsPreviousCartBaudRate;

void MainWindow::EnteringSettingsPage() {
  gSettingsPreviousLogPath = ui->LogPathLineEdit->text();
  gSettingsPreviousSensorSerialPort =
      ui->SensorSerialPortComboBox->currentText();
  gSettingsPreviousSensorBaudRate =
      ui->SensorSerialBaudRateComboBox->currentText();
  gSettingsPreviousCartSerialPort = ui->CartSerialPortComboBox->currentText();
  gSettingsPreviousCartBaudRate = ui->CartSerialBaudRateComboBox->currentText();

  FillSerialPortComboBox(*ui->SensorSerialPortComboBox);
  FillSerialPortComboBox(*ui->CartSerialPortComboBox);
}

void MainWindow::LeavingSettingsPage() {
  if (ui->SensorSerialBaudRateComboBox->currentText() !=
          gSettingsPreviousSensorBaudRate ||
      ui->SensorSerialPortComboBox->currentText() !=
          gSettingsPreviousSensorSerialPort) {
    ResetSensorDriver();
  }

  if (ui->CartSerialBaudRateComboBox->currentText() !=
          gSettingsPreviousCartBaudRate ||
      ui->CartSerialPortComboBox->currentText() !=
      gSettingsPreviousCartSerialPort) {
    ResetCartDriver();
  }
}

void MainWindow::on_SettingsRefreshPushButton_clicked()
{
  FillSerialPortComboBox(*ui->SensorSerialPortComboBox);
  FillSerialPortComboBox(*ui->CartSerialPortComboBox);
}

void MainWindow::on_SettingsCartDistanceLineEdit_textEdited(const QString &arg1)
{
  ui->CartPageDistanceLineEdit->setText(arg1);
}
