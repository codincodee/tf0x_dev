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

class QPushButton;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void timerEvent(QTimerEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

private:
  void SetupMenuButtons();
  void SetMenuButtonUI(QPushButton* button);
  void SetupMenuButtonsUI();
  void ClearAllMenuButtons();
  void ConnectDevice();
  void ClearAllConfirmButtons();
  void ClearAllOptionButtons();
  void ClearAllNoneMenuButtons();

  Ui::MainWindow *ui;
  QPoint mouse_event_offset_;
  std::vector<QPushButton*> menu_buttons_;
  std::vector<QPushButton*> option_buttons_;
  std::vector<QPushButton*> confirm_buttons_;
  QString kCommonStyleSheet;
  QString kConfirmButtonText;
  int kMenuButtonFontSize;
};

#endif // MAIN_WINDOW_H
