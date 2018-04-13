#include "main_window.h"
#include "ui_main_window.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  QLineSeries *series = new QLineSeries();

  series->append(0, 6);
  series->append(2, 4);
  series->append(3, 8);
  series->append(7, 4);
  series->append(10, 5);
  *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

  QChart *chart = new QChart();
  chart->legend()->hide();
  chart->addSeries(series);
  chart->createDefaultAxes();
  chart->setTitle("Simple line chart example");

  QChartView *chartView = new QChartView(chart);
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
  qDebug() << dist;
}
