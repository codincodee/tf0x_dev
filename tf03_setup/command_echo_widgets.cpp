#include "command_echo_widgets.h"
#include <QComboBox>
#include <QDebug>
#include "driver.h"

CommandEchoWidgetsManager::CommandEchoWidgetsManager()
  : kSetButtonText{"Set", "设置"} {
  {
    auto combo = new QComboBox;
    auto button = new QPushButton;
    Lingual devel{"Devel", "开发"};
    Lingual release{"Release", "发布"};
    AddWidgets(
        0x44,
        {new QLabel, combo, button, new QLabel,
         {"Protocol", "协议"},
         kSetButtonText,
         [combo, devel, release](){
           combo->clear();
           combo->addItem(which_lingual(devel));
           combo->addItem(which_lingual(release));
         }});
    connect(button, &QPushButton::clicked, [this, combo, devel, release](){
      if (lingual_equal(combo->currentText(), devel)) {
        driver_->SetDevelMode();
      } else if (lingual_equal(combo->currentText(), release)) {
        driver_->SetReleaseMode();
      } else {
        qDebug() << "Error: " << __FUNCTION__ << __LINE__;
      }
    });
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
