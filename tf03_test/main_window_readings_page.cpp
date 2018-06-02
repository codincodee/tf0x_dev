#include "main_window.h"
#include "ui_main_window.h"
#include <QChartView>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

void MainWindow::InitializeReadingsPage() {
  main_chart_ = new tf0x_common::DistanceOverTimeChart();
  main_chart_->SetCeiling(150.0f);
  main_chart_->SetFloor(5.0f);
  main_chart_view_ = new QtCharts::QChartView(main_chart_);
  ui->MainChartVerticalLayout->addWidget(main_chart_view_);
  numeric_display_timer_.start();
}

void MainWindow::HandleIncomingMeasurement(
    const tf03_driver::Measurement &measurement) {
  if (measurement.dists.empty()) {
    return;
  }
  CacheReadingsLog({measurement.dists[0] / 100.0f});
  main_chart_->AddPoint(measurement.dists[0] / 100.0f, measurement.ts);
  auto series = main_chart_->Series();
  if (!series) {
    return;
  }
  if (!series->count()) {
    return;
  }
  if (numeric_display_timer_.elapsed() > 100) {
    ui->ReadingsPageDistLabel->setText(
        QString::number(series->at(series->count() - 1).y(), 'f', 2));
    float standard_deviation, average;
    main_chart_->CurrentAverageAndStandardDeviation(average, standard_deviation);
    ui->ReadingPageAverageLabel->setText(QString::number(average, 'f', 2));
    ui->ReadingsPageSDLabel->setText(
        QString::number(standard_deviation, 'f', 2));
    numeric_display_timer_.restart();
  }
}

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
    QFile file(
        ui->LogPathLineEdit->text() + "/" + "readings_log_" + file_name + ".txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      QTextStream stream(&file);
      for (auto& entry : readings_log_) {
        stream << entry.dist << "\n";
      }
    } else {
      QMessageBox::warning(this, "Error", "Unable to write log file.", QMessageBox::Abort);
    }
    readings_log_.clear();
    ui->ReadingsPageRecordPushButton->setText(kReadingPageRecordButtonRecord);
    ui->ReadingsPageLogFileLineEdit->setDisabled(false);
  }
}

void MainWindow::on_ReadingsPageBrowsePushButton_clicked()
{
  QDesktopServices::openUrl(ui->LogPathLineEdit->text());
}

void MainWindow::CacheReadingsLog(const ReadingsLog &readings) {
  if (ui->ReadingsPageRecordPushButton->text() !=
      kReadingPageRecordButtonRecord) {
    return;
  }
  readings_log_.push_back(readings);
}
