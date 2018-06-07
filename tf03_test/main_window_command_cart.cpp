#include "main_window.h"
#include "ui_main_window.h"
#include <QMessageBox>

void MainWindow::InitializeCommandPageCartSection() {

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

