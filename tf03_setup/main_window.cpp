#include "main_window.h"
#include "ui_main_window.h"
#include "driver.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  driver_.reset(new Driver);
  driver_->Open();
}

MainWindow::~MainWindow()
{
  driver_->Close();
}
