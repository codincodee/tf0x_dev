#include "cart_driver.h"
#include "utils.h"
#include <iostream>

using utils = tf0x_driver::Utils;

namespace cart_driver {

#if 0
bool Driver::Initialize() {
  current_ = -0.5;
  return true;
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> serial) {
  serial_port_ = serial;
}

bool Driver::ReadInstruction(Instruction &instruction, int &repetition) {
  repetition = 1;
  return ReadInstruction(instruction);
}

bool Driver::ReadInstruction(Instruction &instruction) {
  if (timer_.elapsed() < 10) {
    return false;
  }
  if (current_ < 0.0f && current_ > -1.0) {
    instruction.type = Instruction::Type::reach_start_point;
    return true;
  }
  if (current_ < 0.0f) {
    current_ += delta_;
    return false;
  }
  instruction.type = Instruction::Type::read_sensor;
  timer_.restart();
  current_ += delta_;
  if (current_ >= distance_) {
    instruction.type = Instruction::Type::reach_end_point;
    current_ = -distance_;
  }
  return true;
}

bool Driver::StartMultiStopsTesting(const float &distance, const float &delta) {
  distance_ = distance;
  delta_ = delta;
  current_ = 0;
  timer_.restart();
  return true;
}

float Driver::TotalDistance() {
  return distance_;
}

float Driver::StopInterval() {
  return delta_;
}

#else
bool Driver::Initialize() {
  return true;
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> serial) {
  serial_port_ = serial;
}

bool Driver::ReadInstruction(Instruction &instruction, int &repetition) {
  if (!serial_port_) {
    return false;
  }
  std::string buffer;
  if (!serial_port_->ReadBuffer(buffer)) {
    return false;
  }
  if (buffer.size() < 5) {
    return false;
  }
  std::string alpha(buffer.begin(), buffer.begin() + 4);
  if (alpha != utils::ToBuffer(std::vector<unsigned char>{0x50, 0x4f, 0x53, 0x49})) {
    return false;
  }
  switch (buffer[4]) {
  case 0x11: instruction.type = Instruction::Type::read_sensor; break;
  case 0x12: instruction.type = Instruction::Type::reach_end_point; break;
  case 0x13: instruction.type = Instruction::Type::reach_start_point; break;
  default: return false;
  }
  if (buffer.size() == 5) {
    repetition = 1;
    return true;
  }
  if (buffer.size() % 5 == 0) {
    repetition = buffer.size() / 5;
  }
  return true;
}

bool Driver::ReadInstruction(Instruction &instruction) {
  if (!serial_port_) {
    return false;
  }
  std::string buffer;
  if (!serial_port_->ReadBuffer(buffer)) {
    return false;
  }
  if (buffer.size() < 5) {
    return false;
  }
  for (auto& c : buffer) {
    std::cout << utils::ToHexString(c) << " ";
  }
  std::cout << std::endl;
  std::string alpha(buffer.begin(), buffer.end() - 1);
  if (alpha != utils::ToBuffer(std::vector<unsigned char>{0x50, 0x4f, 0x53, 0x49})) {
    return false;
  }
  switch (buffer[4]) {
  case 0x11: instruction.type = Instruction::Type::read_sensor; break;
  case 0x12: instruction.type = Instruction::Type::reach_end_point; break;
  case 0x13: instruction.type = Instruction::Type::reach_start_point; break;
  default: return false;
  }
  return true;
}

bool Driver::StartMultiStopsTesting(const float &distance, const float &delta) {
  if (!serial_port_) {
    return false;
  }
  distance_ = distance;
  delta_ = delta;
  auto buffer =
      utils::ToBuffer(std::vector<unsigned char>{0x50, 0x4f, 0x53, 0x49, 0x01}) +
      utils::ToBuffer(int32_t(distance * 100)) +
      utils::ToBuffer(int32_t(delta * 100)) +
      utils::ToBuffer(int32_t(1)) +
      utils::ToBuffer(int32_t(1));
  return serial_port_->WriteBuffer(buffer);
}

float Driver::TotalDistance() {
  return distance_;
}

float Driver::StopInterval() {
  return delta_;
}

#endif
} // namespace cart_driver
