#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "tf0x_drivers/tf02_driver.h"
#include <QtCharts/qchart.h>
#include "tf0x_common/distance_over_time_chart.h"
#include <QElapsedTimer>
#include "tf0x_common/tf02_traffic_count.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void on_ResetPushButton_clicked();

private:
  Ui::MainWindow *ui;
  int timer_id_;
  tf02_driver::Driver driver_;
  tf0x_common::DistanceOverTimeChart* chart_;
  QElapsedTimer elapsed_timer_;
  int i;
  std::shared_ptr<tf02_common::TrafficCount> traffic_count_;
};

#endif // MAIN_WINDOW_H
