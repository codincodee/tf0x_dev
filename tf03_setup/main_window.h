#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>

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
  void on_TestPushButton_clicked();

private:
  Ui::MainWindow *ui;
  std::shared_ptr<Driver> driver_;
  int timer_id_;
};

#endif // MAIN_WINDOW_H
