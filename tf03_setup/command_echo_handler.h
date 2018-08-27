#ifndef COMMAND_ECHO_HANDLER_H
#define COMMAND_ECHO_HANDLER_H

#include <memory>
#include <unordered_map>

class Driver;
class QString;

class CommandEchoHandler
{
public:
  CommandEchoHandler();
  void SetDriver(std::shared_ptr<Driver> driver);
  void Probe();
  bool IsCommandEchoed(const char& id);
  bool IsCommandSucceeded(const char& id);
  bool IsFrequencyEchoed();
  unsigned short Frequency();
  bool IsSerialNumberEchoed();
  QString SerialNumber();
  bool IsOutputSwitchEchoed();
  bool IsOutputOn();
  bool IsBaudRateEchoed();
  int BaudRate();
private:
  std::shared_ptr<Driver> driver_;
  std::unordered_map<char, bool> echo_map_;
  std::vector<unsigned short> frequencies_;
  std::vector<std::pair<bool, QString>> serial_numbers_;
  std::vector<bool> output_status_;
  std::vector<int> baud_rates_;
};

#endif // COMMAND_ECHO_HANDLER_H
