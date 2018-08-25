#include "command_echo_widgets_manager.h"
#include <QGridLayout>
#include "command_echo_widgets.h"
#include <QLabel>
#include <QPushButton>

CommandEchoWidgetsManager::CommandEchoWidgetsManager() {
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
    ++row;
  }
}

void CommandEchoWidgetsManager::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void CommandEchoWidgetsManager::SetEchoHandler(
    std::shared_ptr<CommandEchoHandler> handler) {
  echo_handler_ = handler;
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
  std::vector<std::shared_ptr<CommandEchoWidgets>> widgets;

  #ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
  widgets.push_back(
      std::shared_ptr<SetProtocolWidgets>(new SetProtocolWidgets));
  #endif
  widgets.push_back(
      std::shared_ptr<SetFrequencyWidgets>(new SetFrequencyWidgets));
  widgets.push_back(
      std::shared_ptr<SerialNumberWidgets>(new SerialNumberWidgets));

  for (auto& widget : widgets) {
    widget->driver = driver_;
    widget->echo_handler = echo_handler_;
    AddWidgets(widget->id, widget);
  }

  SetUIGrid(ui_grid_);
}
