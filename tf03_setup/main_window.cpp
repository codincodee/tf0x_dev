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
  timer_id_ = startTimer(100);
}

MainWindow::~MainWindow()
{
  driver_->Close();
}


void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  MeasureBasic measure;
  if (driver_->LastMeasure(measure)) {
    ui->DistanceDisplayLabel->setText(QString::number(measure.dist));
  }
  auto message = driver_->GetMessages();
  if (!message.empty()) {
    message[0];
  }
}

void MainWindow::on_TestPushButton_clicked()
{
  static bool release = true;
  release = !release;
  if (release) {
    driver_->SetReleaseMode();
  } else {
    driver_->SetDevelMode();
  }
}
