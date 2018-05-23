#include "cart_driver.h"

namespace cart_driver {
bool Driver::Initialize() {
  current_ = -0.5;
  return true;
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> serial) {
  serial_port_ = serial;
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
} // namespace cart_driver
