#ifndef COMMAND_ECHO_WIDGETS_MANAGER_H
#define COMMAND_ECHO_WIDGETS_MANAGER_H

#include <memory>
#include <unordered_map>
#include "lingual.h"

class Driver;
class QGridLayout;
struct CommandEchoWidgets;
class CommandEchoHandler;

class CommandEchoWidgetsManager {
 public:
  CommandEchoWidgetsManager();
  void SetUIGrid(QGridLayout *layout);
  void SetDriver(std::shared_ptr<Driver> driver);
  void SetEchoHandler(std::shared_ptr<CommandEchoHandler> handler);
  void LoadWidgets();

  void AddWidgets(const std::shared_ptr<CommandEchoWidgets>& widget);
  void UpdateUITexts();
  void Update();
 private:
  void SetupUIGrid(QGridLayout* layout);
  std::vector<std::shared_ptr<CommandEchoWidgets>> widgets_;
  QGridLayout* ui_grid_;
  std::shared_ptr<Driver> driver_;
  std::shared_ptr<CommandEchoHandler> echo_handler_;
};
#endif // COMMAND_ECHO_WIDGETS_MANAGER_H
