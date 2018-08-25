#ifndef COMMAND_ECHO_HANDLER_H
#define COMMAND_ECHO_HANDLER_H

#include <memory>
#include <unordered_map>

class Driver;

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
private:
  std::shared_ptr<Driver> driver_;
  std::unordered_map<char, bool> echo_map_;
  std::vector<unsigned short> frequencies_;
};

#endif // COMMAND_ECHO_HANDLER_H
