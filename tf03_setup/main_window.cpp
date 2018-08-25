#include "main_window.h"
#include "ui_main_window.h"
#include "driver.h"
#include <QElapsedTimer>
#include <QComboBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  driver_.reset(new Driver);
  driver_->Open();
  timer_id_ = startTimer(100);
  frequency_timer_.start();

  command_echo_widgets_manager_.reset(new CommandEchoWidgetsManager);
  command_echo_widgets_manager_->SetUIGrid(ui->CommandEchoGridLayout);
  command_echo_widgets_manager_->SetDriver(driver_);
  command_echo_widgets_manager_->LoadWidgets();

  SetupUIText();
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
      last_freq_measure_id_ = measure.id;
      frequency_timer_.restart();
    }
  }

  command_echo_widgets_manager_->Update();
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

void MainWindow::on_ChinesePushButton_clicked()
{
  set_current_language(Language::chinese);
  SetupUIText();
}

void MainWindow::on_EnglishPushButton_clicked()
{
  set_current_language(Language::english);
  SetupUIText();
}

void MainWindow::SetupUIText() {
  ui->LanguageLabel->setText(which_lingual(kLanguageLabelText));
  ui->DistanceHintLabel->setText(which_lingual(kDistanceLabelText));
  ui->FrequencyHintLabel->setText(which_lingual(kFrequencyLabelText));

  if (command_echo_widgets_manager_) {
    command_echo_widgets_manager_->UpdateUITexts();
  }
}
