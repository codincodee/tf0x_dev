#ifndef COMMAND_ECHO_WIDGETS_H
#define COMMAND_ECHO_WIDGETS_H

#include <QLabel>
#include <QPushButton>
#include <unordered_map>
#include <QGridLayout>
#include "lingual.h"
#include <memory>
#include <QElapsedTimer>
#include <QComboBox>

#include "driver.h"
#include <QDebug>

struct CommandEchoWidgets : public QObject
{
  CommandEchoWidgets();

  QLabel* item;
  QWidget* option;
  QPushButton* button;
  QLabel* status;

  Lingual item_lingual;
  Lingual button_lingual;
  virtual void SetOptionLingual();
  Lingual status_lingual;

  virtual void Update();

  Lingual kButtonLingual = {"Set", "设置"};
  Lingual kNoResponseLingual = {"No Response", "未响应"};

  QElapsedTimer timer;

  Q_OBJECT

 public slots:
  void OnButtonClicked();

 protected:
  virtual void ButtonClicked();
};

struct SetProtocolWidgets : public CommandEchoWidgets {
  SetProtocolWidgets();
  void SetOptionLingual();
  void ButtonClicked();
  Lingual devel = {"Devel", "开发"};
  Lingual release = {"Release", "发布"};
  std::shared_ptr<Driver> driver;
  QComboBox* combo;
};

#endif // COMMAND_ECHO_WIDGETS_H
