#include "tf02_traffic_driver.h"

namespace tf02_traffic_driver {
bool Driver::Initialize() {
  serial_port_.reset(new tf0x_driver::QtSerialPort);
  serial_port_->SetBaudRate(115200);
  serial_port_->SetDataBits(8);
  serial_port_->SetStopBits(1);
  serial_port_->SetPortName(port_name_);
  return serial_port_->Initialize();
}

bool Driver::ReadPrediction(bool &high) {
  if (!serial_port_) {
    return false;
  }
  std::string buffer;
  serial_port_->ReadBuffer(buffer);
  if (buffer.size() < 1) {
    high = false;
    return true;
  }
  high = false;
  for (auto& c : buffer) {
    if (c == 0x00) {
      high = true;
    }
  }
  return true;
}

bool Driver::SetPortName(const std::string &name) {
  port_name_ = name;
  return true;
}
} // namespace tf02_traffic_driver
