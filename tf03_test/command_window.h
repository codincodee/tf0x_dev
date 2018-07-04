#ifndef COMMAND_WINDOW_H
#define COMMAND_WINDOW_H

#include <QMainWindow>
#include "main_window.h"

namespace Ui {
  class CommandWindow;
}

class CommandWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit CommandWindow(MainWindow *parent = 0);
  ~CommandWindow();
  void SetMainWindow(MainWindow* win);
private slots:
  void on_SetAPDPushButton_clicked();

  void on_SetVdbsPushButton_clicked();

  void on_SetCorrAPushButton_clicked();

  void on_SetCorrBPushButton_clicked();

private:
  Ui::CommandWindow *ui;
  MainWindow* main_window_;
};

#endif // COMMAND_WINDOW_H
