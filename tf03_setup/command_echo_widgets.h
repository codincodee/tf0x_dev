#ifndef COMMAND_ECHO_WIDGETS_H
#define COMMAND_ECHO_WIDGETS_H

#include <QLabel>
#include <QPushButton>
#include <unordered_map>
#include <QGridLayout>
#include "lingual.h"

struct CommandEchoWidgets
{
  QLabel* item;
  QWidget* option;
  QPushButton* button;
  QLabel* status;

  Lingual item_lingual;
  Lingual button_lingual;
  std::function<void()> option_lingual;
  Lingual status_lingual;
};

class CommandEchoWidgetsManager {
 public:
  CommandEchoWidgetsManager();
  void SetUIGrid(QGridLayout* layout);
  void AddWidgets(const char& id, const CommandEchoWidgets& widget);
  void UpdateUITexts();
 private:
  std::unordered_map<char, CommandEchoWidgets> widgets_;
  QGridLayout* ui_grid_;
  const Lingual kSetButtonText;
};

#endif // COMMAND_ECHO_WIDGETS_H
