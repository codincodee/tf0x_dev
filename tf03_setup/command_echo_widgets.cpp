#include "command_echo_widgets.h"
#include <QComboBox>
#include <QDebug>
#include "driver.h"

CommandEchoWidgetsManager::CommandEchoWidgetsManager()
  : kSetButtonText{"Set", "设置"} {
  {
    Lingual devel{"Devel", "开发"};
    Lingual release{"Release", "发布"};
    const CommandEchoWidgets const_widgets;
    auto combo = new QComboBox;
    auto button = new QPushButton;
    auto status = new QLabel;
    CommandEchoWidgets widgets;
    std::shared_ptr<QElapsedTimer> timer(new QElapsedTimer);
    widgets.timer = timer;
    widgets.item = new QLabel;
    widgets.button = button;
    widgets.status = status;
    widgets.option = combo;
    widgets.item_lingual = {"Protocol", "通信协议"};
    widgets.button_lingual = const_widgets.kButtonText;
    widgets.option_lingual = [combo, devel, release](){
      combo->clear();
      combo->addItem(which_lingual(devel));
      combo->addItem(which_lingual(release));
    };
    widgets.update = [timer, button, status, const_widgets](){
      if (button->isEnabled()) {
        return;
      }
      if (timer->elapsed() > 1000) {
        button->setDisabled(false);
        status->setText(which_lingual(const_widgets.kNoResponse));
      }
    };

    connect(
        button, &QPushButton::clicked,
        [this, button, timer, combo, devel, release, status](){
      button->setDisabled(true);
      status->clear();
      timer->restart();
      if (lingual_equal(combo->currentText(), devel)) {
        driver_->SetDevelMode();
      } else if (lingual_equal(combo->currentText(), release)) {
        driver_->SetReleaseMode();
      } else {
        qDebug() << "Error: " << __FUNCTION__ << __LINE__;
      }
    });
    AddWidgets(0x44, widgets);
  }
}

void CommandEchoWidgetsManager::AddWidgets(
    const char& id, const CommandEchoWidgets &widget) {
  widgets_[id] = widget;
}

void CommandEchoWidgetsManager::SetUIGrid(QGridLayout *layout) {
  ui_grid_ = layout;

  int row = 0;
  auto grid = ui_grid_;
  for (auto& widgets : widgets_) {
    int column = 0;
    grid->addWidget(widgets.second.item, row, column++);
    grid->addWidget(widgets.second.option, row, column++);
    grid->addWidget(widgets.second.button, row, column++);
    grid->addWidget(widgets.second.status, row, column++);
  }
}

void CommandEchoWidgetsManager::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void CommandEchoWidgetsManager::UpdateUITexts() {
  for (auto& widgets : widgets_) {
    widgets.second.item->setText(which_lingual(widgets.second.item_lingual));
    widgets.second.button->setText(which_lingual(widgets.second.button_lingual));
    widgets.second.option_lingual();
    widgets.second.status->setText(which_lingual(widgets.second.status_lingual));
  }
}

void CommandEchoWidgetsManager::Update() {
  for (auto& widgets : widgets_) {
    widgets.second.update();
  }
}
