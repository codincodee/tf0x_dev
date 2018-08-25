#include "command_echo_widgets.h"
#include <QComboBox>
#include <QDebug>
#include "driver.h"

CommandEchoWidgetsManager::CommandEchoWidgetsManager()
  : kSetButtonText{"Set", "设置"} {
}

void CommandEchoWidgetsManager::AddWidgets(
    const char& id, const std::shared_ptr<CommandEchoWidgets> &widget) {
  widgets_[id] = widget;
}

void CommandEchoWidgetsManager::SetUIGrid(QGridLayout *layout) {
  ui_grid_ = layout;

  int row = 0;
  auto grid = ui_grid_;
  for (auto& widgets : widgets_) {
    int column = 0;
    grid->addWidget(widgets.second->item, row, column++);
    grid->addWidget(widgets.second->option, row, column++);
    grid->addWidget(widgets.second->button, row, column++);
    grid->addWidget(widgets.second->status, row, column++);
  }
}

void CommandEchoWidgetsManager::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void CommandEchoWidgetsManager::UpdateUITexts() {
  for (auto& widgets : widgets_) {
    widgets.second->item->setText(which_lingual(widgets.second->item_lingual));
    widgets.second->button->setText(which_lingual(widgets.second->button_lingual));
    widgets.second->SetOptionLingual();
    widgets.second->status->setText(which_lingual(widgets.second->status_lingual));
  }
}

void CommandEchoWidgetsManager::Update() {
  for (auto& widgets : widgets_) {
    widgets.second->Update();
  }
}

void CommandEchoWidgetsManager::LoadWidgets() {
  std::shared_ptr<SetProtocolWidgets> widgets(new SetProtocolWidgets);
  widgets->driver = driver_;
  AddWidgets(0x44, widgets);

  SetUIGrid(ui_grid_);
}
