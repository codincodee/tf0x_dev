#include "main_window.h"
#include "ui_main_window.h"
#include <qDebug>

void MainWindow::InitializeCartPage() {
  cart_chart_ = new tf0x_common::FixedDistanceOverDistanceChart();
  cart_chart_view_ = new QtCharts::QChartView(cart_chart_);
  ui->CartChartVerticalLayout->addWidget(cart_chart_view_);

  cart_chart_->SetXRange(0, 150);
  cart_chart_->SetYRange(0, 150);
  cart_driver_->StartMultiStopsTesting(100, 0.01);
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
      }
    }
  }
}
