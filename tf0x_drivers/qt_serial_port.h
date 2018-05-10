#ifndef QT_SERIAL_PORT_H
#define QT_SERIAL_PORT_H

#include "abstract_serial_port.h"
#include <QSerialPort>
#include <memory>

namespace tf0x_driver {
class QtSerialPort : public AbstractSerialPort {
 public:
  bool Initialize();
  bool ReadBuffer(std::string &buffer) override;
  bool WriteBuffer(const std::string &buffer) override;
  static QSerialPort::StopBits ConvertToQtStopBits(const int& integer);
  static QSerialPort::DataBits ConvertToQtDataBits(const int& integer);
private:
  std::unique_ptr<QSerialPort> q_serial_port_;
};
} // namespace tf0x_driver

#endif // QT_SERIAL_PORT_H
