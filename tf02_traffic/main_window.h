#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "tf0x_drivers/tf02_driver.h"
#include <QtCharts/qchart.h>
#include <QtCharts/qlineseries.h>

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
  Ui::MainWindow *ui;
  int timer_id_;
  tf02_driver::Driver driver_;
  QtCharts::QChart* chart_;
  QtCharts::QLineSeries * series_;
};

#endif // MAIN_WINDOW_H
