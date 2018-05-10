#ifndef ABSTRACT_SERIAL_PORT_H
#define ABSTRACT_SERIAL_PORT_H

#include <string>
#include "export.h"

namespace tf0x_driver {
class API AbstractSerialPort {
 public:
  virtual ~AbstractSerialPort();
  void SetBaudRate(const int& baud_rate);
  int GetBaudRate() const;
  void SetDataBits(const int& data_bits);
  int GetDataBits() const;
  void SetStopBits(const int& stop_bits);
  int GetStopBits() const;
  void SetPortName(const std::string& port_name);
  std::string GetPortName() const;
  virtual bool ReadBuffer(std::string& buffer) = 0;
  virtual bool WriteBuffer(const std::string& buffer) = 0;
 private:
  int baud_rate_;
  int data_bits_;
  int stop_bits_;
  std::string port_name_;
};
} // namespace tf0x_driver

#endif // ABSTRACT_SERIAL_PORT_H
