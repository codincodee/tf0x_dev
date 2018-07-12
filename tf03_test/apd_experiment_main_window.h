#ifndef VDBS_EXPERIMENT_MAIN_WINDOW_H
#define VDBS_EXPERIMENT_MAIN_WINDOW_H

#include <QMainWindow>
#include "main_window.h"

namespace Ui {
  class APDExperimentMainWindow;
}

class APDExperimentMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit APDExperimentMainWindow(MainWindow *parent = 0);
  ~APDExperimentMainWindow();

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void on_StartPushButton_clicked();

private:
  Ui::APDExperimentMainWindow *ui;
  MainWindow* main_window_;
};

#endif // VDBS_EXPERIMENT_MAIN_WINDOW_H
