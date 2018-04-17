#include "tf02_driver.h"
#include <iostream>
#include <QSerialPortInfo>

namespace tf02_driver {
bool Driver::ReadDistance(double &distance) {
  if (!serial_port_) {
    return false;
  }
  std::string buffer;
  serial_port_->ReadBuffer(buffer);
  if (buffer.size() < 8) {
    return false;
  }
  if (buffer[0] != 0x59) {
    return false;
  }
  if (buffer[1] != 0x59) {
    return false;
  }
  short dist;
  memcpy(&dist, &buffer[0] + 2, 2);
  distance = dist / 100.0;
  if (distance > 30.0 || distance < 0.0) {
    return false;
  }
  return true;
}

bool Driver::SetPortName(const std::string &name) {
  port_name_ = name;
  return true;
}

bool Driver::Initialize() {
  serial_port_.reset(new tf0x_driver::QtSerialPort);
  serial_port_->SetBaudRate(115200);
  serial_port_->SetDataBits(8);
  serial_port_->SetStopBits(1);
  serial_port_->SetPortName(port_name_);
  return serial_port_->Initialize();
}
} // namespace tf02_driver
