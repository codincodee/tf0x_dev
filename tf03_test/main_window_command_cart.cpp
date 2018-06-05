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
  sensor_driver_->SetAPD(apd);
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
  sensor_driver_->SetVdbs(value);
  return;
}

void MainWindow::on_CommandPageResetPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_->ResetDevice();
}

void MainWindow::on_CommandPageSaveSettingsPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_->SaveSettings();
}

void MainWindow::on_CommandPageRestoreFactoryPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_->RestoreFactory();
}
