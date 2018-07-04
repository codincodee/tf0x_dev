#include "command_window.h"
#include "ui_command_window.h"

CommandWindow::CommandWindow(MainWindow *parent) :
  QMainWindow(parent),
  ui(new Ui::CommandWindow)
{
  ui->setupUi(this);
  SetMainWindow(parent);
}

CommandWindow::~CommandWindow()
{
  delete ui;
}


void CommandWindow::SetMainWindow(MainWindow *win) {
  main_window_ = win;
}

void CommandWindow::on_SetAPDPushButton_clicked()
{
   main_window_->HandleCommandPageSetAPD(ui->SetAPDLineEdit->text());
}

void CommandWindow::on_SetVdbsPushButton_clicked()
{
  main_window_->HandleCommandPageSetVdbs(ui->SetVdbsLineEdit->text());
}

void CommandWindow::on_SetCorrAPushButton_clicked()
{
  main_window_->HandleCommandPageSetCorrA(ui->SetCorrALineEdit->text());
}

void CommandWindow::on_SetCorrBPushButton_clicked()
{
  main_window_->HandleCommandPageSetCorrB(ui->SetCorrBLineEdit->text());
}
