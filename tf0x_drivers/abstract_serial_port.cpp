#include "abstract_serial_port.h"

namespace tf0x_driver {
AbstractSerialPort::~AbstractSerialPort() {}
void AbstractSerialPort::SetBaudRate(const int& baud_rate) {
  baud_rate_ = baud_rate;
}

int AbstractSerialPort::GetBaudRate() const {
  return baud_rate_;
}

void AbstractSerialPort::SetDataBits(const int &data_bits) {
  data_bits_ = data_bits;
}

int AbstractSerialPort::GetDataBits() const {
  return data_bits_;
}

void AbstractSerialPort::SetStopBits(const int &stop_bits) {
  stop_bits_ = stop_bits;
}

int AbstractSerialPort::GetStopBits() const {
  return stop_bits_;
}

void AbstractSerialPort::SetPortName(const std::string &port_name) {
  port_name_ = port_name;
}

std::string AbstractSerialPort::GetPortName() const {
  return port_name_;
}

std::vector<int> AbstractSerialPort::BaudRateLists() {
  return
      {110, 300, 600, 1200, 2400, 4800, 9600, 14400,
       19200, 38400, 57600, 115200, 128000, 256000, 460800};
}
} // namespace tf0x_driver
