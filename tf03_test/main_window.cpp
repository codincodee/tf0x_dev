#include "main_window.h"
#include "ui_main_window.h"
#include <QChartView>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  main_chart_view_ = new QtCharts::QChartView;
  ui->MainChartVerticalLayout->addWidget(main_chart_view_);
}

MainWindow::~MainWindow()
{
  delete ui;
}
