#include "qt_serial_port.h"

namespace tf0x_driver {
bool QtSerialPort::Initialize() {
  q_serial_port_.reset(new QSerialPort);
  q_serial_port_->setPortName(QString::fromStdString(GetPortName()));
  q_serial_port_->setBaudRate(GetBaudRate());
  q_serial_port_->setStopBits(ConvertToQtStopBits(GetStopBits()));
  q_serial_port_->setDataBits(ConvertToQtDataBits(GetDataBits()));
  q_serial_port_->open(QIODevice::ReadWrite);
  return true;
}

QSerialPort::StopBits QtSerialPort::ConvertToQtStopBits(const int &integer) {
  switch (integer) {
  case 1: return QSerialPort::OneStop;
  case 2: return QSerialPort::OneAndHalfStop;
  case 3: return QSerialPort::TwoStop;
  default: return QSerialPort::OneStop;
  }
}

QSerialPort::DataBits QtSerialPort::ConvertToQtDataBits(const int &integer) {
  switch (integer) {
  case 5: return QSerialPort::Data5;
  case 6: return QSerialPort::Data6;
  case 7: return QSerialPort::Data7;
  case 8: return QSerialPort::Data8;
  default: return QSerialPort::Data8;
  }
}

bool QtSerialPort::ReadBuffer(std::string &buffer) {
  if (!q_serial_port_) {
    return false;
  }
  if (!q_serial_port_->isOpen()) {
    return false;
  }
  buffer = q_serial_port_->readAll().toStdString();
  return true;
}

bool QtSerialPort::WriteBuffer(const std::string &buffer) {
  if (!q_serial_port_) {
    return false;
  }
  if (!q_serial_port_->isOpen()) {
    return false;
  }
  // Ingoring return value
  q_serial_port_->write(buffer.c_str(), buffer.size());
  return true;
}
} // namespace tf0x_driver
