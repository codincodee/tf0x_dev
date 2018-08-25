#ifndef COMMAND_ECHO_WIDGETS_MANAGER_H
#define COMMAND_ECHO_WIDGETS_MANAGER_H

#include <memory>
#include <unordered_map>
#include "lingual.h"

class Driver;
class QGridLayout;
struct CommandEchoWidgets;

class CommandEchoWidgetsManager {
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
  std::shared_ptr<Driver> driver_;
};
#endif // COMMAND_ECHO_WIDGETS_MANAGER_H
