#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <tf0x_common/distance_over_time_chart.h>
#include <tf0x_drivers/tf03_driver.h>
#include <tf0x_drivers/qt_serial_port.h>
#include <QComboBox>
#include <tf0x_drivers/cart_driver.h>
#include <tf0x_common/distance_over_distance_chart.h>
#include <QElapsedTimer>
#include <tf0x_common/cart_test_multi_result_sheets.h>
#include <thread>
#include <mutex>
#include <atomic>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void HandleCommandPageSetVdbs(const QString& input);
  void HandleCommandPageSetAPD(const QString& input);
  void HandleCommandPageSetCorrB(const QString& input);
  void HandleCommandPageSetCorrA(const QString& input);
protected:
  void timerEvent(QTimerEvent *event);
private slots:
  void on_LogPathPushButton_clicked();

  void on_tabWidget_currentChanged(int index);

  void on_DebugSendPushButton_clicked();

  void on_SettingsRefreshPushButton_clicked();

  void on_CartStartTestPushButton_clicked();

  void on_SettingsCartDistanceLineEdit_textEdited(const QString &arg1);

  void on_CartPageDistanceLineEdit_textEdited(const QString &arg1);

  void on_CartPageBrowsePushButton_clicked();

  void on_ReadingsPageRecordPushButton_clicked();

  void on_ReadingsPageBrowsePushButton_clicked();

  void on_CommandPageSetAPDPushButton_clicked();

  void on_CommandPageSetVdbsPushButton_clicked();

  void on_CommandPageResetPushButton_clicked();

  void on_CommandPageSaveSettingsPushButton_clicked();

  void on_CommandPageRestoreFactoryPushButton_clicked();

  void on_CommandPageSetFrequencyPushButton_clicked();

  void on_SettingsPageVisualizationFrequencyComboBox_currentTextChanged(const QString &arg1);

  void on_CommandPageSetCorrAPushButton_clicked();

  void on_CommandPageSetCorrBPushButton_clicked();

  void on_CommandPageSetProtocolPushButton_clicked();

  void on_CommandPageSetTransTypePushButton_clicked();

  void on_CommandPageWriteParamPushButton_clicked();

  void on_CommandPageCheckVersionPushButton_clicked();

  void on_CommandPageVdbsAdjustPushButton_clicked();

  void on_CommandPageAPDExperimentPushButton_clicked();

  void on_CommandPageAPDAutoPushButton_clicked();

  void on_ReadingsPageDistanceSwtichPushButton_clicked();

  void on_CommandPageAPDClosedLoopPushButton_clicked();

  void on_CommandPageAutoGainPushButton_clicked();

  void on_CommandPageTDCOutRangeValuePushButton_clicked();

  void on_CommandPageCANSendIDPushButton_clicked();

  void on_CommandPageCANReceiveIDPushButton_clicked();

  void on_CommandPageCANBaudRatePushButton_clicked();

signals:
  void ShowAPDExperimentWindow();
public:
  bool SetSensorSerialPort(tf0x_driver::AbstractSerialPort& port);
  static void FillComboBoxWithBaudRate(QComboBox& combo_box);
  static void FillSerialPortComboBox(QComboBox& combo_box);
  void InitializeSettingsPage();
  void InitializeTimerEvent();
  void InitializeReadingsPage();
  void InitializeCommandPageCartSection();
  void InitializeCartPage();
  void LoadSettingsFromConfigFile();
  void SaveSettingsToConfigFile();
  static QString ConfigFilePath();
  void ResetSensorDriver();
  void ResetCartDriver();
  void HandleIncomingStream(const std::string& buffer);
  void EnteringSettingsPage();
  void LeavingSettingsPage();
  void PrintInDebugWindow(const std::string& title, const std::string& content);
  void PrintInDebugWindow(const QString &title, const QString& content);
  void HandleSensorTimerEvent();
  void HandleCartTimerEvent();
  void HandleCartInstruction(
      const cart_driver::Instruction& instruction);
  void HandleCartInstruction(
      const cart_driver::Instruction &instruction, const int& repetition);
  void SaveCartTestLog(const QString& suffix = "");
  void HandleIncomingMeasurement(const tf03_driver::Measurement& measurement);
  bool SaveReadingsLog();
  void SaveCartTestLog(const std::vector<tf03_driver::CartMeasurement>& readings);
  void CacheReadingsLog(const tf03_driver::Measurement& readings);
  void HandleSerialDetectionEvent();

  void SensorThread();
  void CartThread();

  void HandleCommandPageEchoUpdate();
  void CommandPageDumpEcho(const QString& msg);

  void HandleAPDExperiment(const std::vector<tf03_driver::Measurement>& measurements);

  Ui::MainWindow *ui;
  QtCharts::QChartView* main_chart_view_;
  tf0x_common::DistanceOverTimeChart* main_chart_;
  std::shared_ptr<tf03_driver::Driver> sensor_driver_;
  std::shared_ptr<tf0x_driver::QtSerialPort> sensor_serial_;
  std::shared_ptr<cart_driver::Driver> cart_driver_;
  std::shared_ptr<tf0x_driver::QtSerialPort> cart_serial_;
  int previous_page_;
  tf03_driver::Measurement last_measurement_;
  QElapsedTimer elapsed_timer_;
  QElapsedTimer numeric_display_timer_;
  std::list<tf03_driver::Measurement> readings_log_;
  std::vector<tf03_driver::Measurement> measurement_cache_;
  std::shared_ptr<tf0x_common::CartTestMultiResultSheets> cart_results_;
  std::vector<tf03_driver::CartMeasurement> cart_log_;

  // Multi-threads
  std::shared_ptr<std::thread> sensor_thread_;
  std::mutex sensor_driver_mutex_;
  std::atomic_bool sensor_thread_exit_signal_;
  std::mutex sensor_readings_mutex_;
  std::vector<tf03_driver::Measurement> sensor_readings_;
  std::atomic_bool sensor_logging_;
  std::mutex sensor_log_mutex_;
  std::list<tf03_driver::Measurement> sensor_log_;
  std::mutex sensor_last_measurement_mutex_;
  tf03_driver::Measurement sensor_last_measurement_;
  std::atomic<float> sensor_frequency_;

  std::shared_ptr<std::thread> cart_thread_;
  std::mutex cart_driver_mutex_;
  std::atomic_bool cart_thread_exit_signal_;
  std::mutex cart_readings_mutex_;
  std::vector<tf03_driver::CartMeasurement> cart_readings_;
  std::atomic_bool cart_logging_;
  std::mutex cart_last_measurement_mutex_;
  tf03_driver::CartMeasurement cart_last_measurement_;

  bool sensor_serial_pending_reconnect_ = false;

  QString kVisualizationFrequencyHigh = "High";
  QString kVisualizationFrequencyLow = "Low";

  int cycle_timer_high_;
  int cycle_timer_low_;
  int cycle_timer_;

  bool apd_experiment_on_ = false;
  std::atomic<float> apd_anticipated_voltage_;
  float apd_experiment_step_voltage_;
  float apd_experiment_step_interval_;
  float apd_voltage_from_;
  float apd_voltage_to_;
  QElapsedTimer apd_experiment_timer_;
  // std::mutex apd_experiment_measurements_mutex_;
  std::list<tf03_driver::Measurement> apd_experiment_measurements_list_;
  std::list<tf03_driver::Measurement>::iterator apd_experiment_measurements_list_iterator_;
  std::map<int, std::shared_ptr<std::list<tf03_driver::Measurement>>> apd_voltage_map_;
  float apd_stand_dist_;
  bool IsAPDCrashed(const std::list<tf03_driver::Measurement>& list);
  std::atomic_bool apd_experiment_ended_;
  std::atomic_bool apd_crashed_;
  std::atomic<float> apd_crashed_voltage_;

  bool readings_page_rawdist_display_;

  int RobustAverageDist(std::shared_ptr<std::list<tf03_driver::Measurement>> dists);
  int RobustSTD(std::shared_ptr<std::list<tf03_driver::Measurement>> dists, const int& std_dist);
  const int kMaxToIgnore = 2;
  const int kMinToIngore = 2;
};

#endif // MAIN_WINDOW_H
