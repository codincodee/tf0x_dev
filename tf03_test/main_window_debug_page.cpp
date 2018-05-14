#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/utils.h>
#include <QDebug>

void MainWindow::HandleIncomingStream(const std::string &buffer) {
  if (buffer.empty()) {
    return;
  }
  ui->DebugReceivePlainTextEdit->appendPlainText(QString::fromStdString(buffer));
}

void MainWindow::on_DebugSendPushButton_clicked()
{
  if (!sensor_serial_) {
    return;
  }
  auto cmd = ui->DebugSendLineEdit->text().toStdString();
  if (cmd.empty()) {
    return;
  }
  auto buffer = tf0x_driver::Utils::CommandLineInputToBuffer(cmd);
  qDebug() << QString::fromStdString(buffer) << buffer.size();
  sensor_serial_->WriteBuffer(buffer);
}
