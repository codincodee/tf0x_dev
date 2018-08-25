#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <QElapsedTimer>
#include "lingual.h"
#include "command_echo_widgets_manager.h"
#include <unordered_map>

class Driver;

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
  void SetupUIText();

  void on_ChinesePushButton_clicked();

  void on_EnglishPushButton_clicked();

private:
  Ui::MainWindow *ui;
  std::shared_ptr<Driver> driver_;
  int timer_id_;
  QElapsedTimer frequency_timer_;
  unsigned long last_freq_measure_id_ = 0;
  const Lingual kLanguageLabelText = {"Language", "语言"};
  const Lingual kDistanceLabelText = {"Distance (cm)", "距离 （厘米）"};
  const Lingual kFrequencyLabelText = {"Frequency (hz)", "频率 （赫兹）"};
  std::shared_ptr<CommandEchoWidgetsManager> command_echo_widgets_manager_;
};

#endif // MAIN_WINDOW_H
