#include "main_window.h"
#include "ui_main_window.h"
#include <QDebug>
#include <QMessageBox>

void MainWindow::InitializeCartPage() {
  cart_chart_ = new tf0x_common::FixedDistanceOverDistanceChart();
  cart_chart_view_ = new QtCharts::QChartView(cart_chart_);
  ui->CartChartVerticalLayout->addWidget(cart_chart_view_);

  cart_chart_->SetXRange(0, 150);
  cart_chart_->SetYRange(0, 150);
}

void MainWindow::HandleCartInstruction(
    const cart_driver::Instruction &instruction) {
  if (instruction.type == cart_driver::Instruction::Type::read_sensor) {
    auto series = cart_chart_->Series();
    if (series) {
      QPointF last;
      if (series->count()) {
        last = series->at(series->count() - 1);
      } else {
        last = QPointF(0, 0);
      }
      last_measurement_.dists.push_back(50);
      if (!last_measurement_.dists.empty()) {
        auto current_pose = last.x() + cart_driver_->StopInterval();
        cart_chart_->AddPoint(current_pose, last_measurement_.dists[0]);
        ui->CartInfoLabel->setText("Collected " + QString::number(series->count()) + " Points.");
      }
    }
  } else if (instruction.type == cart_driver::Instruction::Type::reach_end_point) {
    ui->CartInfoLabel->setText("Finished.");
  }
}

void MainWindow::on_CartStartTestPushButton_clicked()
{
  cart_chart_->Clear();
  if (!cart_driver_) {
    return;
  }
  bool distance_ok, interval_ok;
  auto distance = ui->SettingsCartDistanceLineEdit->text().toFloat(&distance_ok);
  auto interval = ui->SettingsCartIntervalLineEdit->text().toFloat(&interval_ok);
  if (!distance_ok || !interval_ok) {
    return;
  }
  cart_driver_->StartMultiStopsTesting(distance, interval);
}
