#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <thread>
#include <atomic>
#include <mutex>
#include <tf0x_drivers/tf03_driver.h>

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
  bool OnStart();
  bool OnStop();
  void timerEvent(QTimerEvent *event);

private slots:
  void on_StartPushButton_clicked();

  void on_FreshPushButton_clicked();

private:
  void ReadingThread(std::vector<std::string> serials);
  void SaveReadings();
  std::atomic_bool stop_signal_;
  Ui::MainWindow *ui;
  const QString kStartPushButtonStart;
  const QString kStartPushButtonStop;
  std::thread thread_;
  std::vector<std::list<tf03_driver::Measurement>> readings_;
  std::mutex latest_measurements_mutex_;
  std::vector<tf03_driver::Measurement> latest_measurements_;

  std::vector<std::shared_ptr<tf0x_driver::AbstractSerialPort>> serial_ports;
  std::vector<std::shared_ptr<tf03_driver::Driver>> drivers;
};

#endif // MAIN_WINDOW_H
