#include "main_window.h"
#include "ui_main_window.h"
#include <qDebug>

void MainWindow::InitializeCartPage() {
  cart_chart_ = new tf0x_common::FixedDistanceOverDistanceChart();
  cart_chart_view_ = new QtCharts::QChartView(cart_chart_);
  ui->CartChartVerticalLayout->addWidget(cart_chart_view_);

  cart_chart_->SetXRange(0, 150);
  cart_chart_->SetYRange(0, 150);
}

void MainWindow::HandleCartInstruction(
    const cart_driver::Instruction &instruction) {

}
