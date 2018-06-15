#include "main_window.h"
#include "ui_main_window.h"
#include <QMessageBox>

const QString kCommandSetProtocolReleaseProtocol = "Release";
const QString kCommandSetProtocolDevelopProtocol = "Develop";

const QString kCommandSetTransTypeSerial = "Serial";
const QString kCommandSetTransTypeCAN = "CAN";

void MainWindow::InitializeCommandPageCartSection() {
  ui->CommandPageSetProtocolComboBox->addItem(kCommandSetProtocolReleaseProtocol);
  ui->CommandPageSetProtocolComboBox->addItem(kCommandSetProtocolDevelopProtocol);

  ui->CommandPageSetTransTypeComboBox->addItem(kCommandSetTransTypeCAN);
  ui->CommandPageSetTransTypeComboBox->addItem(kCommandSetTransTypeSerial);
}

void MainWindow::on_CommandPageSetAPDPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int apd = ui->CommandPageSetAPDLineEdit->text().toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  if (apd > 200 || apd < 90) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetAPD(apd);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetVdbsPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = ui->CommandPageSetVdbsLineEdit->text().toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetVdbs(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageResetPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->ResetDevice();
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageSaveSettingsPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SaveSettings();
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageRestoreFactoryPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->RestoreFactory();
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageSetFrequencyPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = ui->CommandPageSetFrequencyLineEdit->text().toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetFrequency(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetCorrAPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  double value = ui->CommandPageSetCorrALineEdit->text().toDouble(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetTableCorrA(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetCorrBPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = ui->CommandPageSetCorrBLineEdit->text().toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetTableCorrB(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetProtocolPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  tf03_driver::ProtocolType type;
  if (ui->CommandPageSetProtocolComboBox->currentText() ==
      kCommandSetProtocolReleaseProtocol) {
    type = tf03_driver::ProtocolType::release;
  } else if (ui->CommandPageSetProtocolComboBox->currentText() ==
             kCommandSetProtocolDevelopProtocol) {
    type = tf03_driver::ProtocolType::develop;
  } else {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }

  sensor_driver_mutex_.lock();
  sensor_driver_->SetProtocolType(type);
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageSetTransTypePushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  tf03_driver::TransType type;
  if (ui->CommandPageSetTransTypeComboBox->currentText() ==
      kCommandSetTransTypeSerial) {
    type = tf03_driver::TransType::serial;
  } else if (ui->CommandPageSetTransTypeComboBox->currentText() ==
             kCommandSetTransTypeCAN) {
    type = tf03_driver::TransType::can;
  } else {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }

  sensor_driver_mutex_.lock();
  sensor_driver_->SetTransType(type);
  sensor_driver_mutex_.unlock();
}
