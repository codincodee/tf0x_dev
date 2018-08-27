#include "command_echo_handler.h"
#include "driver.h"
#include "static_unique_ptr_cast.h"

CommandEchoHandler::CommandEchoHandler()
{

}

void CommandEchoHandler::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void CommandEchoHandler::Probe() {
  if (!driver_) {
    return;
  }
  echo_map_.clear();
  frequencies_.clear();
  serial_numbers_.clear();
  output_status_.clear();
  auto message = driver_->GetMessages();
  for (auto& msg : message) {
    if (msg.type == MessageType::status) {
      auto status = static_unique_ptr_cast<StatusEcho>(std::move(msg.data));
      if (status) {
        echo_map_[status->cmd_id] = status->success;
      }
    } else if (msg.type == MessageType::frequency) {
      auto freq = static_unique_ptr_cast<FrequencyEcho>(std::move(msg.data));
      if (freq) {
        frequencies_.push_back(freq->frequency);
      }
    } else if (msg.type == MessageType::serial_number) {
      auto sn = static_unique_ptr_cast<SerialNumberEcho>(std::move(msg.data));
      if (sn) {
        serial_numbers_.push_back({sn->status, sn->sn});
      }
    } else if (msg.type == MessageType::output_switch) {
      auto status =
          static_unique_ptr_cast<OutputSwitchEcho>(std::move(msg.data));
      if (status) {
        output_status_.push_back(status->on);
      }
    }
  }
}

bool CommandEchoHandler::IsCommandEchoed(const char &id) {
  return echo_map_.count(id);
}

bool CommandEchoHandler::IsCommandSucceeded(const char &id) {
  if (!IsCommandEchoed(id)) {
    return false;
  }
  return echo_map_[id];
}

bool CommandEchoHandler::IsFrequencyEchoed() {
  return !frequencies_.empty();
}

unsigned short CommandEchoHandler::Frequency() {
  if (!IsFrequencyEchoed()) {
    return 0;
  }
  return *frequencies_.rbegin();
}

bool CommandEchoHandler::IsSerialNumberEchoed() {
  return !serial_numbers_.empty();
}

QString CommandEchoHandler::SerialNumber() {
  if (!IsSerialNumberEchoed()) {
    return "";
  }
  if (!serial_numbers_.rbegin()->first) {
    return "";
  }
  return serial_numbers_.rbegin()->second;
}

bool CommandEchoHandler::IsOutputSwitchEchoed() {
  return !output_status_.empty();
}

bool CommandEchoHandler::IsOutputOn() {
  if (!IsOutputSwitchEchoed()) {
    return false;
  }
  return *output_status_.rbegin();
}
