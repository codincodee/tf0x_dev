#include "main_window.h"
#include "ui_main_window.h"
#include <QPushButton>
#include <QDebug>
#include <qserialportinfo.h>

void MainWindow::SetupMenuButtons() {
  QPushButton* button;
  QPoint coor(0, 0);

  coor.setY(coor.y() + 1);
  button = new QPushButton;
  menu_buttons_.push_back(button);
  button->setText("连接设备");
  ui->GridLayout->addWidget(button, coor.y(), coor.x());
  connect(button, &QPushButton::clicked, [this, coor](){
    ClearAllNoneMenuButtons();
    auto ports = QSerialPortInfo::availablePorts();
    auto co = coor;
    co.setX(co.x() + 1);
    for (auto& port : ports) {
      QPushButton* button = new QPushButton;
      ui->GridLayout->addWidget(button, co.y(), co.x());
      option_buttons_.push_back(button);
      auto name = port.portName();
      button->setText(name);
      SetMenuButtonUI(button);
      connect(button, &QPushButton::clicked, [this, co, name](){
        ClearAllConfirmButtons();
        QPushButton* button = new QPushButton;
        ui->GridLayout->addWidget(button, co.y(), co.x() + 1);
        confirm_buttons_.push_back(button);
        button->setText(kConfirmButtonText);
        SetMenuButtonUI(button);
        connect(button, &QPushButton::clicked, [this, name](){
          ClearAllConfirmButtons();
          ClearAllOptionButtons();
          qDebug() << name;
        });
      });
      co.setY(co.y() + 1);
    }
  });

  coor.setY(coor.y() + 1);
  button = new QPushButton;
  menu_buttons_.push_back(button);
  button->setText("端口模式");
  ui->GridLayout->addWidget(button, coor.y(), coor.x());
  connect(button, &QPushButton::clicked, [this, coor](){
    ClearAllNoneMenuButtons();
    auto co = coor;
    co.setX(co.x() + 1);
    QPushButton* button;
    QString name;

    button = new QPushButton;
    name = "标准串口";
    button->setText(name);
    ui->GridLayout->addWidget(button, co.y(), co.x());
    option_buttons_.push_back(button);
    SetMenuButtonUI(button);
    connect(button, &QPushButton::clicked, [this, co, name](){
      ClearAllConfirmButtons();
      QPushButton* button = new QPushButton;
      ui->GridLayout->addWidget(button, co.y(), co.x() + 1);
      confirm_buttons_.push_back(button);
      button->setText(kConfirmButtonText);
      SetMenuButtonUI(button);
      connect(button, &QPushButton::clicked, [this, name](){
        ClearAllConfirmButtons();
        ClearAllOptionButtons();
        qDebug() << name;
      });
    });

    co.setY(co.y() + 1);
    button = new QPushButton;
    name = "Pixhawk";
    button->setText(name);
    ui->GridLayout->addWidget(button, co.y(), co.x());
    option_buttons_.push_back(button);
    SetMenuButtonUI(button);
    connect(button, &QPushButton::clicked, [this, co, name](){
      ClearAllConfirmButtons();
      QPushButton* button = new QPushButton;
      ui->GridLayout->addWidget(button, co.y(), co.x() + 1);
      confirm_buttons_.push_back(button);
      button->setText(kConfirmButtonText);
      SetMenuButtonUI(button);
      connect(button, &QPushButton::clicked, [this, name](){
        ClearAllConfirmButtons();
        ClearAllOptionButtons();
        qDebug() << name;
      });
    });
  });

  coor.setY(coor.y() + 1);
  button = new QPushButton;
  menu_buttons_.push_back(button);
  button->setText("退出");
  ui->GridLayout->addWidget(button, coor.y(), coor.x());
  connect(button, &QPushButton::clicked, [](){
    QApplication::exit(0);
  });

  SetupMenuButtonsUI();
}

void MainWindow::SetupMenuButtonsUI() {
  for (auto& button : menu_buttons_) {
    SetMenuButtonUI(button);
  }
}

void MainWindow::SetMenuButtonUI(QPushButton* button) {
  button->setStyleSheet(kCommonStyleSheet);
  auto font = button->font();
  font.setPointSize(kMenuButtonFontSize);
  font.setBold(true);
  button->setFont(font);
}

void MainWindow::ClearAllMenuButtons() {
  for (auto& button : menu_buttons_) {
    delete button;
  }
  menu_buttons_.clear();
}

void MainWindow::ClearAllConfirmButtons() {
  for (auto& button : confirm_buttons_) {
    delete button;
  }
  confirm_buttons_.clear();
}

void MainWindow::ClearAllOptionButtons() {
  for (auto& button : option_buttons_) {
    delete button;
  }
  option_buttons_.clear();
}

void MainWindow::ClearAllNoneMenuButtons() {
  ClearAllOptionButtons();
  ClearAllConfirmButtons();
}
