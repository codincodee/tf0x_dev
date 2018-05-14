#include "main_window.h"
#include "ui_main_window.h"

void MainWindow::HandleIncomingStream(const std::string &buffer) {
  if (buffer.empty()) {
    return;
  }
  ui->DebugReceivePlainTextEdit->appendPlainText(QString::fromStdString(buffer));
}
