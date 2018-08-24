#include "main_window.h"
#include "ui_main_window.h"
#include "driver.h"
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  driver_.reset(new Driver);
  driver_->Open();
  timer_id_ = startTimer(100);
  frequency_timer_.start();
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
    auto elapse = frequency_timer_.elapsed();
    if (elapse > 1000) {
      auto frequency =
          (measure.id - last_freq_measure_id_) / (elapse / 1000.0f);
      ui->FrequencyDisplayLabel->setText(QString::number(frequency, 'f', 2));
    }
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
