#include "main_window.h"
#include "ui_main_window.h"
#include <QChartView>

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
  main_chart_->AddPoint(measurement.dists[0], elapsed_timer_.elapsed());
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
    ui->ReadingsPageSDLabel->setText(
        QString::number(main_chart_->CurrentStandardDeviation(), 'f', 2));
    numeric_display_timer_.restart();
  }
}
