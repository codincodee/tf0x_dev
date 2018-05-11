#include "main_window.h"
#include "ui_main_window.h"
#include <QChartView>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->resize(800, 800);
  main_chart_ = new tf0x_common::DistanceOverTimeChart();
  main_chart_view_ = new QtCharts::QChartView(main_chart_);
  ui->MainChartVerticalLayout->addWidget(main_chart_view_);
}

MainWindow::~MainWindow()
{
  delete ui;
  if (main_chart_) {
    delete main_chart_;
    main_chart_ = nullptr;
  }
  if (main_chart_view_) {
    delete main_chart_view_;
    main_chart_view_ = nullptr;
  }
}
