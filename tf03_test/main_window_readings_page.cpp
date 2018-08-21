#include "main_window.h"
#include "ui_main_window.h"
#include <QChartView>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>

const QString kDistanceSwitchPushButtonRawDist = "Plot RawDist";
const QString kDistanceSwitchPushButtonCorrDist = "Plot CorrDist";

void MainWindow::InitializeReadingsPage() {
  main_chart_ = new tf0x_common::DistanceOverTimeChart();
  main_chart_->SetCeiling(150.0f);
  main_chart_->SetFloor(5.0f);
  main_chart_view_ = new QtCharts::QChartView(main_chart_);
  ui->MainChartVerticalLayout->addWidget(main_chart_view_);
  numeric_display_timer_.start();
  ui->ReadingsPageDistanceSwtichPushButton->setText(kDistanceSwitchPushButtonRawDist);
  readings_page_rawdist_display_ = false;
}

//void MainWindow::HandleIncomingMeasurement(
//    const tf03_driver::Measurement &measurement) {
//  CacheReadingsLog(measurement);
//  main_chart_->AddPoint(measurement.dist1 / 100.0f, measurement.ts);
//  auto series = main_chart_->Series();
//  if (!series) {
//    return;
//  }
//  if (!series->count()) {
//    return;
//  }
//  if (numeric_display_timer_.elapsed() > 100) {
//    ui->ReadingsPageDistLabel->setText(
//        QString::number(measurement.dist1 / 100.0f));
//    ui->ReadingsPageDistance2Label->setText(
//        QString::number(measurement.dist2 / 100.0f));
//    ui->ReadingsPageDistance3Label->setText(
//        QString::number(measurement.dist3 / 100.0f));
//    ui->ReadingsPageAPDVoltageLabel->setText(
//        QString::number(measurement.apd));
//    ui->ReadingsPageLaserVoltageLabel->setText(
//        QString::number(measurement.volt));
//    ui->ReadingsPageTemperatureLabel->setText(
//        QString::number(measurement.temp));

//    float standard_deviation, average;
//    main_chart_->CurrentAverageAndStandardDeviation(average, standard_deviation);
//    ui->ReadingPageAverageLabel->setText(QString::number(average, 'f', 2));
//    ui->ReadingsPageSDLabel->setText(
//        QString::number(standard_deviation, 'f', 2));
//    numeric_display_timer_.restart();
//  }
//}

const QString kReadingPageRecordButtonRecord = "Record";
const QString kReadingPageRecordButtonStop = "Stop";

void MainWindow::on_ReadingsPageRecordPushButton_clicked()
{
  if (ui->ReadingsPageRecordPushButton->text() ==
      kReadingPageRecordButtonRecord) {
    if (ui->ReadingsPageLogFileLineEdit->text().isEmpty()) {
      QMessageBox::warning(
          this, "Error",
          "Please enter a valid log file name first.", QMessageBox::Ok);
      return;
    }
    ui->ReadingsPageRecordPushButton->setText(kReadingPageRecordButtonStop);
    ui->ReadingsPageLogFileLineEdit->setDisabled(true);
    sensor_logging_ = true;
  } else if (ui->ReadingsPageRecordPushButton->text() ==
             kReadingPageRecordButtonStop) {
    auto file_name = ui->ReadingsPageLogFileLineEdit->text();
    if (file_name.isEmpty()) {
      QMessageBox::warning(
          this, "Error",
          "Log file name invalid.", QMessageBox::Ok);
      ui->ReadingsPageRecordPushButton->setText(kReadingPageRecordButtonRecord);
      ui->ReadingsPageLogFileLineEdit->setDisabled(false);
      return;
    }
    sensor_logging_ = false;
    sensor_log_mutex_.lock();
    auto logs = *sensor_log_;
    sensor_log_->clear();
    sensor_log_mutex_.unlock();
//    QFile file(
//        ui->LogPathLineEdit->text() + "/" + "readings_log_" + file_name + ".txt");
//    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//      QTextStream stream(&file);
//      stream << "# Raw-Distance-1(cm) Raw-Distance-2(cm) Raw-Distance-3(cm) Distance-1(cm) Distance-2(cm) Distance-3(cm) APD-Voltage(V) Laser-Voltage(V) Algorithm Temperature(C)\n";
//      for (auto& entry : logs) {
//        stream
//            << entry.raw_dist1 << " "
//            << entry.raw_dist2 << " "
//            << entry.raw_dist3 << " "
//            << entry.dist1 << " "
//            << (short)entry.dist2 << " "
//            << entry.dist3 << " "
//            << entry.apd << " "
//            << entry.volt << " "
//            << entry.algorithm << " "
//            << entry.temp << "\n";
//      }

//    } else {
//      QMessageBox::warning(this, "Error", "Unable to write log file.", QMessageBox::Abort);
//    }
    SaveLog(logs, LogFileName());
    ui->ReadingsPageRecordPushButton->setText(kReadingPageRecordButtonRecord);
    ui->ReadingsPageLogFileLineEdit->setDisabled(false);
  }
}

void MainWindow::on_ReadingsPageBrowsePushButton_clicked()
{
  QDesktopServices::openUrl(ui->LogPathLineEdit->text());
}

void MainWindow::CacheReadingsLog(const tf03_driver::Measurement &readings) {
  if (ui->ReadingsPageRecordPushButton->text() !=
      kReadingPageRecordButtonRecord) {
    return;
  }
  readings_log_.push_back(readings);
}

void MainWindow::HandleSensorTimerEvent() {
  sensor_last_measurement_mutex_.lock();
  auto measurement = sensor_last_measurement_;
  sensor_last_measurement_mutex_.unlock();
  float display_dist;
  if (readings_page_rawdist_display_) {
    display_dist = measurement.raw_dist1;
  } else {
    display_dist = measurement.dist1;
  }
  main_chart_->AddPoint(display_dist / 100.0f, measurement.ts);

  if (numeric_display_timer_.elapsed() > 100) {
    ui->ReadingsPageDistLabel->setText(
        QString::number(measurement.dist1 / 100.0f));
    ui->ReadingsPageDistance2Label->setText(
        QString::number((short)measurement.dist2 / 100.0f));
    ui->ReadingsPageDistance3Label->setText(
        QString::number(measurement.dist3 / 100.0f));
    ui->ReadingsPageAPDVoltageLabel->setText(
        QString::number(measurement.apd));
    ui->ReadingsPageLaserVoltageLabel->setText(
        QString::number(measurement.volt));
    ui->ReadingsPageTemperatureLabel->setText(
        QString::number(measurement.temp));
    ui->ReadingsPageRawDist1Label->setText(
        QString::number(measurement.raw_dist1 / 100.0f));
//    ui->ReadingsPageRawDist2Label->setText(
//        QString::number(measurement.raw_dist2 / 100.0f));
//    ui->ReadingsPageRawDist3Label->setText(
//        QString::number(measurement.raw_dist3 / 100.0f));
    ui->ReadingsPageRawDist2Label->setText(
        QString::number((short)measurement.raw_dist2));
    ui->ReadingsPageRawDist3Label->setText(
        QString::number((short)measurement.raw_dist3));
    ui->ReadingsPageAlgorithmLabel->setText(
        QString::number((short)measurement.algorithm));

    float standard_deviation, average;
    main_chart_->CurrentAverageAndStandardDeviation(average, standard_deviation);
    ui->ReadingPageAverageLabel->setText(QString::number(average, 'f', 2));
    ui->ReadingsPageSDLabel->setText(
        QString::number(standard_deviation, 'f', 2));
    numeric_display_timer_.restart();
    ui->ReadingsPageFrequencyLabel->setText(
        QString::number(sensor_frequency_.load(), 'f', 2));
  }
}

void MainWindow::on_ReadingsPageDistanceSwtichPushButton_clicked()
{
  auto button = ui->ReadingsPageDistanceSwtichPushButton;
  if (button->text() == kDistanceSwitchPushButtonRawDist) {
    button->setText(kDistanceSwitchPushButtonCorrDist);
    readings_page_rawdist_display_ = true;
  } else {
    button->setText(kDistanceSwitchPushButtonRawDist);
    readings_page_rawdist_display_ = false;
  }
}

void MainWindow::HandlePeriodicSave() {
  if (!sensor_logging_) {
    return;
  }
  sensor_log_mutex_.lock();
  if (sensor_log_->size() < 50000) {
    sensor_log_mutex_.unlock();
    return;
  }
  auto log = sensor_log_;
  sensor_log_.reset(new std::list<tf03_driver::Measurement>);
  sensor_log_mutex_.unlock();

  SaveLog(*log, LogFileName());
}

QString MainWindow::LogFileName() {
  auto file_name = ui->ReadingsPageLogFileLineEdit->text();
  return
      ui->LogPathLineEdit->text() + "/" + "readings_log_" + file_name + ".txt";
}

void MainWindow::SaveLog(
    const std::list<tf03_driver::Measurement> &log,
    const QString& file_name) {
  QFile file(file_name);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
    QTextStream stream(&file);
    for (auto& entry : log) {
      stream
          << entry.raw_dist1 << " "
          << entry.raw_dist2 << " "
          << entry.raw_dist3 << " "
          << entry.dist1 << " "
          << (short)entry.dist2 << " "
          << entry.dist3 << " "
          << entry.apd << " "
          << entry.volt << " "
          << entry.algorithm << " "
          << entry.temp << "\n";
    }
  }
}
