#include "main_window.h"
#include "ui_main_window.h"
#include <tf0x_drivers/qt_serial_port.h>
#include <tf0x_drivers/tf03_driver.h>
#include <QDebug>
#include <iostream>
#include <QDir>
#include <QFile>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  kCommonStyleSheet("background-color: grey; color: black"),
  kMenuButtonFontSize(12)
{
  ui->setupUi(this);
  startTimer(0);
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint);
  this->statusBar()->setSizeGripEnabled(false);
  ui->DistanceLabel->setStyleSheet(kCommonStyleSheet);

  menu_buttons_.push_back(ui->HardwareInterfacePushButton);
  menu_buttons_.push_back(ui->SetFrequencyPushButton);
  menu_buttons_.push_back(ui->SetSerialPortPushButton);

  for (auto& button : menu_buttons_) {
    button->setStyleSheet(kCommonStyleSheet);
    auto font = button->font();
    font.setPointSize(kMenuButtonFontSize);
    font.setBold(true);
    button->setFont(font);
  }
}

MainWindow::~MainWindow()
{
}

void MainWindow::timerEvent(QTimerEvent *event) {
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  if(event->buttons() & Qt::LeftButton)
  {
      this->move(mapToParent(event->pos() - mouse_event_offset_));
  }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  mouse_event_offset_ = event->pos();
}
