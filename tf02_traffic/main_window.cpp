#include "main_window.h"
#include "ui_main_window.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QDebug>
#include "tf0x_common/distance_over_time_chart.h"
#include <QValueAxis>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  series_ = new QLineSeries();

  chart_ = new tf0x_common::DistanceOverTimeChart();
  chart_->legend()->hide();
  chart_->addSeries(series_);
  // chart_->createDefaultAxes();
  // chart->setTitle("Simple line chart example");

  QChartView *chartView = new QChartView(chart_);
  chartView->setRenderHint(QPainter::Antialiasing);

  QChartView* chartview = new QChartView;

  ui->ChartVerticalLayout->addWidget(chartView);
//  chartView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//  chartView->setMaximumSize(400, 400);
//  chartView->setMinimumSize(400, 400);
  // chartView->setVisible(false);

  QCameraViewfinder* camera_view = new QCameraViewfinder;
  QCamera* camera = new QCamera(QCameraInfo::defaultCamera());
  camera->setViewfinder(camera_view);
  camera->start();
  // ui->VideoVerticalLayout->addWidget(camera_view);
  camera_view->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  camera_view->setMaximumSize(400, 400);
  camera_view->setMinimumSize(400, 400);
  // camera_view->setMinimumWidth(800);
//  ui->gridLayout_2->addWidget(camera_view);
//  ui->gridLayout_2->addWidget(chartView);

  timer_id_ = startTimer(10);

  driver_.Initialize();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return QMainWindow::timerEvent(event);
  }
  double dist;
  if (!driver_.ReadDistance(dist)) {
    return;
  }
  static int cnt = 0;
  ++cnt;
//  if (cnt % 10 != 0) {
//    return;
//  }
  *series_ << QPointF(cnt, dist);

  while (series_->count()) {
    if ((series_->at(0).x() + 500) < cnt) {
      series_->removePoints(0, 1);
    } else {
      break;
    }
  }
  float min = 1000;
  float max = 0;
  for (auto i = 0; i < series_->count(); ++i) {
    if (series_->at(i).y() > max) {
      max = series_->at(i).y();
    }
    if (series_->at(i).y() < min) {
      min = series_->at(i).y();
    }
  }
  if (max > 20) {
    max = 20;
  }
  if (max < 5) {
    max = 5;
  }
  min = 0;
  chart_->removeSeries(series_);
  QValueAxis axis;
  axis.setMax(max);
  axis.setMin(min);
  chart_->addSeries(series_);
  chart_->setAxisY(&axis, series_);
}
