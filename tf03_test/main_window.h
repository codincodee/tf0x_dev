#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <tf0x_common/distance_over_time_chart.h>
#include <tf0x_drivers/tf03_driver.h>
#include <QComboBox>

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
private:
  bool SetSensorSerialPort(tf0x_driver::AbstractSerialPort& port);
  void FillComboBoxWithBaudRate(QComboBox* combo_box);
  void FillSerialPortComboBox(QComboBox* combo_box);
  void SetSettingsPage();
  Ui::MainWindow *ui;
  QtCharts::QChartView* main_chart_view_;
  tf0x_common::DistanceOverTimeChart* main_chart_;
  std::shared_ptr<tf0x_driver::Driver> driver_;
};

#endif // MAIN_WINDOW_H
