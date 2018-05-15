#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/utils.h>
#include <QDebug>
#include <tf0x_common/utils.h>
#include <QDateTime>

void MainWindow::HandleIncomingStream(const std::string &buffer) {
  if (buffer.empty()) {
    return;
  }
  std::string display;
  for (auto& ch : buffer) {
    display += tf0x_common::Utils::ToHexString(ch) + " ";
  }
  PrintInDebugWindow("Received", display);
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
  sensor_serial_->WriteBuffer(buffer);
  PrintInDebugWindow("Send", ui->DebugSendLineEdit->text());
}

void MainWindow::PrintInDebugWindow(
    const QString &title, const QString &content) {
  ui->DebugReceivePlainTextEdit->appendPlainText(
      "[" + title + " at " +
        QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + "] " + content);
}

void MainWindow::PrintInDebugWindow(
    const std::string &title, const std::string &content) {
  PrintInDebugWindow(
      QString::fromStdString(title), QString::fromStdString(content));
}
