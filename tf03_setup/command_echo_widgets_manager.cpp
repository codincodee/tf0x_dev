#include "command_echo_widgets_manager.h"
#include <QGridLayout>
#include "command_echo_widgets.h"
#include <QLabel>
#include <QPushButton>
#include <QDebug>

CommandEchoWidgetsManager::CommandEchoWidgetsManager() {
}

void CommandEchoWidgetsManager::SetUIGrid(QGridLayout *layout) {
  ui_grid_ = layout;
}

void CommandEchoWidgetsManager::AddWidgets(
    const std::shared_ptr<CommandEchoWidgets> &widget) {
  if (!widget) {
    return;
  }
  if (!widget->driver) {
    widget->driver = driver_;
  }
  if (!widget->echo_handler) {
    widget->echo_handler = echo_handler_;
  }
  widgets_.push_back(widget);
}

void CommandEchoWidgetsManager::SetupUIGrid(QGridLayout *layout) {
  auto widget_num = widgets_.size();
  constexpr int kRowMax = 10;
  auto column_num = widgets_.size() / kRowMax + 1;
  auto parent = parent_widget_;

  std::vector<QGridLayout*> layouts;
  {
    int col = 0;
    for (int i = 0; i < column_num; ++i) {
      auto sub_layout = new QGridLayout(parent);
      layouts.push_back(sub_layout);
      if (i != 0) {
        QFrame* vline = new QFrame(parent);
        vline->setFrameShape(QFrame::VLine);
        vline->setLineWidth(1);
        layout->addWidget(vline, 0, col++);
      }
      layout->addLayout(sub_layout, 0, col++);
    }
  }

  int row = 0;
  auto id = 0;
  while (id < widgets_.size()) {
    for (int i = 0; i < column_num; ++i) {
      if (id >= widgets_.size()) {
        break;
      }
      int column = 0;
      layouts[i]->addWidget(widgets_[id]->item, row, column++);
      if (widgets_[id]->option) {
        layouts[i]->addWidget(widgets_[id]->option, row, column++);
      } else {
        ++column;
      }
      layouts[i]->addWidget(widgets_[id]->button, row, column++);
      layouts[i]->addWidget(widgets_[id]->status, row, column++);
      ++id;
    }
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
    widgets->item->setText(which_lingual(widgets->item_lingual));
    widgets->button->setText(which_lingual(widgets->button_lingual));
    widgets->SetOptionLingual();
    widgets->status->setText(which_lingual(widgets->status_lingual));
  }
}

void CommandEchoWidgetsManager::Update() {
  for (auto& widgets : widgets_) {
    widgets->Update();
  }
}

void CommandEchoWidgetsManager::SetParentWidget(QWidget *widget) {
  parent_widget_ = widget;
}

void CommandEchoWidgetsManager::LoadWidgets() {
  #ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
  AddWidgets(
      std::shared_ptr<SetProtocolWidgets>(new SetProtocolWidgets));
  #endif
  AddWidgets(
      std::shared_ptr<SetFrequencyWidgets>(new SetFrequencyWidgets));
  AddWidgets(
      std::shared_ptr<SerialNumberWidgets>(new SerialNumberWidgets));
  AddWidgets(
      std::shared_ptr<OutputSwitchWidgets>(new OutputSwitchWidgets));
  AddWidgets(
      std::shared_ptr<MeasureTriggerWidgets>(new MeasureTriggerWidgets));
  AddWidgets(
      std::shared_ptr<SetSerialBaudRateWidgets>(new SetSerialBaudRateWidgets));
  AddWidgets(
      std::shared_ptr<SetPortTypeWidgets>(new SetPortTypeWidgets));
  AddWidgets(
      std::shared_ptr<SetCANSendIDWidgets>(new SetCANSendIDWidgets));
  AddWidgets(
      std::shared_ptr<SetCANReceiveIDWidgets>(new SetCANReceiveIDWidgets));
  AddWidgets(
      std::shared_ptr<SetCANBaudRateWidgets>(new SetCANBaudRateWidgets));
  AddWidgets(
      std::shared_ptr<SetOutputFormatWidgets>(new SetOutputFormatWidgets));
  AddWidgets(
      std::shared_ptr<SetOutRangeValueWidgets>(new SetOutRangeValueWidgets));
  AddWidgets(
      std::shared_ptr<FlashSettingsWidgets>(new FlashSettingsWidgets));
  AddWidgets(
      std::shared_ptr<RestoreFactoryWidgets>(new RestoreFactoryWidgets));

  if (ui_grid_) {
    SetupUIGrid(ui_grid_);
  }
}
