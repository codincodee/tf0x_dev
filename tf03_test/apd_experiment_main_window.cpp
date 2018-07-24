#include "apd_experiment_main_window.h"
#include "ui_apd_experiment_main_window.h"

const QString kStartPushButtonStart = "Start";
const QString kStartPushButtonStop = "Stop";

APDExperimentMainWindow::APDExperimentMainWindow(MainWindow *parent) :
  QMainWindow(parent),
  main_window_(parent),
  ui(new Ui::APDExperimentMainWindow)
{
  ui->setupUi(this);
  ui->StartPushButton->setText(kStartPushButtonStart);
  ui->StartVoltageLineEdit->setText("150");
  ui->EndVoltageLineEdit->setText("180");
  ui->StepIntervalLineEdit->setText("2");
  ui->StepVoltageLineEdit->setText("1");
  startTimer(500);
}

APDExperimentMainWindow::~APDExperimentMainWindow()
{
  delete ui;
}

void APDExperimentMainWindow::timerEvent(QTimerEvent *event) {
  if (!main_window_->apd_experiment_on_) {
    return;
  }

  ui->ProgressBar->setValue(
      100.0f * (main_window_->apd_anticipated_voltage_ - main_window_->apd_voltage_from_) /
      (main_window_->apd_voltage_to_ - main_window_->apd_voltage_from_));
  ui->StatusLabel->setText("Current Command APD Voltage: " + QString::number(main_window_->apd_anticipated_voltage_));
  if (main_window_->apd_experiment_ended_) {
    main_window_->apd_experiment_on_ = false;
    QString prompt = "Experiment Ended. ";
    if (main_window_->apd_crashed_) {
      prompt += "Found APD crash voltage: " + QString::number(main_window_->apd_crashed_voltage_) + ". ";
    } else {
      prompt += "Failed to find a APD crash voltage. ";
    }
    ui->StatusLabel->setText(prompt);
    ui->StartPushButton->setText(kStartPushButtonStart);
    return;
  }

  if (main_window_->apd_experiment_timer_.elapsed() > main_window_->apd_experiment_step_interval_) {
    main_window_->apd_experiment_timer_.restart();
    main_window_->apd_anticipated_voltage_ = main_window_->apd_anticipated_voltage_ + main_window_->apd_experiment_step_voltage_;
    if (main_window_->apd_anticipated_voltage_ > main_window_->apd_voltage_to_) {
      main_window_->apd_experiment_ended_ = true;
      main_window_->apd_crashed_ = false;
    }
    main_window_->sensor_driver_mutex_.lock();
    main_window_->sensor_driver_->SetAPD(main_window_->apd_anticipated_voltage_);
    main_window_->sensor_driver_mutex_.unlock();
  }
}

void APDExperimentMainWindow::on_StartPushButton_clicked()
{
  if (ui->StartPushButton->text() == kStartPushButtonStart) {
    bool ok;
    main_window_->apd_voltage_from_ = ui->StartVoltageLineEdit->text().toFloat(&ok);
    if (!ok) {
      return;
    }
    main_window_->apd_voltage_to_ = ui->EndVoltageLineEdit->text().toFloat(&ok);
    if (!ok) {
      return;
    }
    main_window_->apd_experiment_step_voltage_ = ui->StepVoltageLineEdit->text().toFloat(&ok);
    if (!ok) {
      return;
    }
    main_window_->apd_experiment_step_interval_ = ui->StepIntervalLineEdit->text().toFloat(&ok) * 1000;
    if (!ok) {
      return;
    }
    main_window_->apd_experiment_ended_ = false;
    main_window_->apd_crashed_ = false;
    main_window_->apd_anticipated_voltage_ = main_window_->apd_voltage_from_;
    main_window_->apd_experiment_measurements_list_.clear();
    main_window_->apd_voltage_map_.clear();
    main_window_->apd_experiment_measurements_list_iterator_ = main_window_->apd_experiment_measurements_list_.begin();
    main_window_->apd_stand_dist_ = -1.0f;
    main_window_->apd_experiment_timer_.restart();
    main_window_->sensor_driver_mutex_.lock();
    main_window_->sensor_driver_->EnableAPDAuto(false);
    main_window_->sensor_driver_mutex_.unlock();
    main_window_->apd_experiment_on_ = true;
    ui->StartPushButton->setText(kStartPushButtonStop);
  } else {
    main_window_->apd_experiment_on_ = false;
    ui->StartPushButton->setText(kStartPushButtonStart);
  }
}
