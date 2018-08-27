#ifndef COMMAND_ECHO_WIDGETS_H
#define COMMAND_ECHO_WIDGETS_H

#include <QObject>
#include "lingual.h"
#include <QElapsedTimer>

class QLabel;
class QPushButton;
class Driver;
class QComboBox;
class CommandEchoHandler;

////////////////////// CommandEchoWidgets /////////////////////////////

struct CommandEchoWidgets : public QObject
{
  CommandEchoWidgets();

  char id;
  QLabel* item;
  QWidget* option;
  QPushButton* button;
  QLabel* status;

  Lingual item_lingual;
  Lingual button_lingual;
  virtual void SetOptionLingual();
  Lingual status_lingual;

  virtual void Update();

  const Lingual kButtonSetLingual = {"Set", "设置"};
  const Lingual kButtonRequestLingual = {"Request", "请求"};
  const Lingual kNoResponseLingual = {"No Response", "未响应"};
  const Lingual kSuccessLingual = {"Successful", "成功"};
  const Lingual kFailLingual = {"Failed", "失败"};
  const Lingual kUnknownLingual = {"Unknown", "未知错误"};

  QElapsedTimer timer;

  std::shared_ptr<Driver> driver;
  std::shared_ptr<CommandEchoHandler> echo_handler;

  int timeout;

  Q_OBJECT

 public slots:
  void OnButtonClicked();

 protected:
  virtual void ButtonClicked();
  static QLabel* UINullLabel();
  static void SetWidgetUINullLabel(QWidget*& widget);
  void SetOptionWidgetUINull();
  void SetStatusLabelUINull();
  const static QString kUINullString;
};

////////////////////// SetProtocolWidgets /////////////////////////////

struct SetProtocolWidgets : public CommandEchoWidgets {
  SetProtocolWidgets();
  void SetOptionLingual() override;
  void ButtonClicked() override;
  const Lingual kDevelLingual = {"Devel", "开发"};
  const Lingual kReleaseLingual = {"Release", "发布"};
  QComboBox* combo;
};

////////////////////// SetFrequencyWidgets /////////////////////////////

struct SetFrequencyWidgets : public CommandEchoWidgets {
  SetFrequencyWidgets();
  void ButtonClicked() override;
  void Update() override;
  QComboBox* combo;
};

////////////////////// SerialNumberWidgets /////////////////////////////

struct SerialNumberWidgets : public CommandEchoWidgets {
  SerialNumberWidgets();
  void ButtonClicked() override;
  void Update() override;
  QLabel* label;
};

////////////////////// OutputSwitchWidgets /////////////////////////////

struct OutputSwitchWidgets : public CommandEchoWidgets {
  OutputSwitchWidgets();
  void ButtonClicked() override;
  void Update() override;
  void SetOptionLingual() override;
  QComboBox* combo;
  const Lingual kSwitchOnLingual = {"ON", "开"};
  const Lingual kSwitchOffLingual = {"OFF", "关"};
  const Lingual kOutputOnLingual = {"Output On", "已开"};
  const Lingual kOutputOffLingual = {"Output Off", "已关"};
};

////////////////////// MeasureTriggerWidgets /////////////////////////////

struct MeasureTriggerWidgets : public CommandEchoWidgets {
  MeasureTriggerWidgets();
  void ButtonClicked() override;
  void Update() override;
};

////////////////////// FlashSettingsWidgets /////////////////////////////

struct FlashSettingsWidgets : public CommandEchoWidgets {
  FlashSettingsWidgets();
  void ButtonClicked() override;
};

////////////////////// RestoreFactoryWidgets /////////////////////////////

struct RestoreFactoryWidgets : public CommandEchoWidgets {
  RestoreFactoryWidgets();
  void ButtonClicked() override;
};

#endif // COMMAND_ECHO_WIDGETS_H
