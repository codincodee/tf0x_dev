#include "cart_driver.h"

namespace cart_driver {
bool Driver::Initialize() {
  return true;
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> serial) {
  serial_port_ = serial;
}

bool Driver::ReadInstruction(Instruction &instruction) {
  return true;
}
} // namespace cart_driver
