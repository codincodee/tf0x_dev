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
  CommandEchoWidgets() {
    item = new QLabel;
    button = new QPushButton;
    status = new QLabel;
    button_lingual = kButtonLingual;
    button->setText(which_lingual(kButtonLingual));
    connect(button, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
  }

  QLabel* item;
  QWidget* option;
  QPushButton* button;
  QLabel* status;

  Lingual item_lingual;
  Lingual button_lingual;
  virtual void SetOptionLingual() {

  }
  Lingual status_lingual;

  virtual void Update() {
    if (button->isEnabled()) {
      return;
    }
    if (timer.elapsed() > 1000) {
      button->setDisabled(false);
      status->setText(which_lingual(kNoResponseLingual));
      status_lingual = kNoResponseLingual;
    }
  }

  Lingual kButtonLingual = {"Set", "设置"};
  Lingual kNoResponseLingual = {"No Response", "未响应"};

  QElapsedTimer timer;

  Q_OBJECT

 public slots:
  void OnButtonClicked() {
    return ButtonClicked();
  }
 protected:
  virtual void ButtonClicked() {}
};

struct SetProtocolWidgets : public CommandEchoWidgets {
  SetProtocolWidgets() : CommandEchoWidgets() {
    item_lingual = {"Protocol", "通信协议"};
    combo = new QComboBox;
    option = combo;
  }
  void SetOptionLingual() override {
    combo->clear();
    combo->addItem(which_lingual(devel));
    combo->addItem(which_lingual(release));
  }
  void ButtonClicked() override {
    button->setDisabled(true);
    status->clear();
    timer.restart();
    if (lingual_equal(combo->currentText(), devel)) {
      driver->SetDevelMode();
    } else if (lingual_equal(combo->currentText(), release)) {
      driver->SetReleaseMode();
    } else {
      qDebug() << "Error: " << __FUNCTION__ << __LINE__;
    }
  }
  Lingual devel = {"Devel", "开发"};
  Lingual release = {"Release", "发布"};
  std::shared_ptr<Driver> driver;
  QComboBox* combo;
};

class CommandEchoWidgetsManager : public QObject {
 public:
  CommandEchoWidgetsManager();
  void SetUIGrid(QGridLayout* layout);
  void SetDriver(std::shared_ptr<Driver> driver);
  void LoadWidgets();

  void AddWidgets(
      const char& id, const std::shared_ptr<CommandEchoWidgets>& widget);
  void UpdateUITexts();
  void Update();
 private:
  std::unordered_map<char, std::shared_ptr<CommandEchoWidgets>> widgets_;
  QGridLayout* ui_grid_;
  const Lingual kSetButtonText;
  std::shared_ptr<Driver> driver_;
};

#endif // COMMAND_ECHO_WIDGETS_H
