#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <tf0x_common/distance_over_time_chart.h>
#include <tf0x_drivers/tf03_driver.h>
#include <tf0x_drivers/qt_serial_port.h>
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
private slots:
  void on_LogPathPushButton_clicked();

  void on_tabWidget_currentChanged(int index);

  void on_DebugSendPushButton_clicked();

  void on_SettingsRefreshPushButton_clicked();

private:
  bool SetSensorSerialPort(tf0x_driver::AbstractSerialPort& port);
  static void FillComboBoxWithBaudRate(QComboBox& combo_box);
  static void FillSerialPortComboBox(QComboBox& combo_box);
  void InitializeSettingsPage();
  void InitializeTimerEvent();
  void InitializeReadingsPage();
  void InitializeCommandPageCartSection();
  void LoadSettingsFromConfigFile();
  void SaveSettingsToConfigFile();
  static QString ConfigFilePath();
  void ResetSensorDriver();
  void HandleIncomingStream(const std::string& buffer);
  void EnteringSettingsPage();
  void LeavingSettingsPage();
  void PrintInDebugWindow(const std::string& title, const std::string& content);
  void PrintInDebugWindow(const QString &title, const QString& content);

  Ui::MainWindow *ui;
  QtCharts::QChartView* main_chart_view_;
  tf0x_common::DistanceOverTimeChart* main_chart_;
  std::shared_ptr<tf03_driver::Driver> sensor_driver_;
  std::shared_ptr<tf0x_driver::QtSerialPort> sensor_serial_;
  int previous_page_;
};

#endif // MAIN_WINDOW_H
