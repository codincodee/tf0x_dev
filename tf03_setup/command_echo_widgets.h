#ifndef COMMAND_ECHO_WIDGETS_H
#define COMMAND_ECHO_WIDGETS_H

#include <QLabel>
#include <QPushButton>
#include <unordered_map>
#include <QGridLayout>
#include "lingual.h"
#include <memory>
#include <QElapsedTimer>

class Driver;

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

  std::function<void()> update;

  Lingual kButtonText = {"Set", "设置"};
  Lingual kNoResponse = {"No Response", "未响应"};

  std::shared_ptr<QElapsedTimer> timer;
};

class CommandEchoWidgetsManager : public QObject {
 public:
  CommandEchoWidgetsManager();
  void SetUIGrid(QGridLayout* layout);
  void SetDriver(std::shared_ptr<Driver> driver);
  void AddWidgets(const char& id, const CommandEchoWidgets& widget);
  void UpdateUITexts();
  void Update();
 private:
  std::unordered_map<char, CommandEchoWidgets> widgets_;
  QGridLayout* ui_grid_;
  const Lingual kSetButtonText;
  std::shared_ptr<Driver> driver_;
};

#endif // COMMAND_ECHO_WIDGETS_H
