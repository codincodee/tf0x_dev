#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "tf0x_drivers/tf02_driver.h"
#include <QtCharts/qchart.h>
#include "tf0x_common/distance_over_time_and_switch_value_chart.h"
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

  void on_SerialPortComboBox_activated(const QString &arg1);

  void on_SettingsSwitchToolButton_clicked();

private:
  void SetSettingsVisable(const bool& visable);
  Ui::MainWindow *ui;
  int timer_id_;
  tf02_driver::Driver driver_;
  tf0x_common::DistanceOverTimeAndSwitchValueChart* chart_;
  QElapsedTimer elapsed_timer_;
  QElapsedTimer rate_elapsed_timer_;
  int i;
  std::shared_ptr<tf02_common::TrafficCount> traffic_count_;
};

#endif // MAIN_WINDOW_H
