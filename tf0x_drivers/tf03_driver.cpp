#include "tf03_driver.h"
#include <iostream>
#include "utils.h"
#include <QElapsedTimer>
#include <stdint.h>

constexpr char kHead = 0x5A;
using utils = tf0x_driver::Utils;

namespace tf03_driver {
#if 0
bool Driver::Initialize() {
  if (!serial_port_) {
    return false;
  }
  timer_.start();
  return true;
}

bool Driver::ReadDistance(double &dist0) {
  Measurement measurement;
  if (!ReadMeasurement(measurement)) {
    return false;
  }
  if (measurement.dists.empty()) {
    return false;
  }
  dist0 = measurement.dists[0] / 100.0;
  return true;
}

bool Driver::ReadMeasurement(Measurement& measurement, std::string& buffer) {
  if (!serial_port_) {
    return false;
  }
  // TODO: Implementations
  return true;
}

bool Driver::ReadMeasurement(Measurement &measurement) {
//  std::string buffer;
//  return ReadMeasurement(measurement, buffer);

  static float dist = 10.0f;
  static float sign = 1.0f;
  if (dist > 150.0f) {
    sign *= -1.0f;
  }
  if (dist < 1.0f) {
    sign *= -1.0f;
  }
  dist += sign * 1;
  measurement.dists.push_back(dist);
  measurement.ts = timer_.elapsed();
  return true;
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> port) {
  serial_port_ = port;
}

std::string Driver::GetVersion() {
  if (!serial_port_) {
    return std::string();
  }
  std::string buffer = Head() + std::string(1, 4) + std::string(1, 1);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  serial_port_->WriteBuffer(AppendCheckSum(buffer));
  int cnt = 0;
  std::string echo;
  while (!serial_port_->ReadBuffer(echo)) {
    if (++cnt > 10) {
      return std::string();
    }
  }
  if (echo.size() < 6) {
    return std::string();
  }
  if (echo[0] != kHead) {
    return std::string();
  }
  if (echo[1] != 7)  {
    return std::string();
  }
  if (echo[2] != 1) {
    return std::string();
  }
  std::string version;
  version =
      utils::ToDecimalString(echo[5]) + "." +
      utils::ToDecimalString(echo[4]) + "." +
      utils::ToDecimalString(echo[3]);
  return version;
}

std::string Driver::Head() {
  return std::string(1, kHead);
}

std::string Driver::AppendCheckSum(const std::string &buffer) {
  int sum = 0;
  for (auto& c : buffer) {
    sum += c;
  }
  sum &= 0xFF;
  return buffer + std::string(1, (char)sum);
}

#else
bool Driver::Initialize() {
  if (!serial_port_) {
    return false;
  }
  return true;
}

bool Driver::ReadDistance(double &dist0) {
  Measurement measurement;
  if (!ReadMeasurement(measurement)) {
    return false;
  }
  dist0 = measurement.dist1;
  return true;
}

bool Driver::ReadMeasurement(Measurement& measurement, std::string& buffer) {
  if (!serial_port_) {
    return false;
  }
  // TODO: Implementations
  if (!serial_port_->ReadBuffer(buffer)) {
    return false;
  }
  if (buffer.size() < 22) {
    return false;
  }
  if (buffer[0] != kHead) {
    return false;
  }
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[10], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[11], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.dist1 = high16;
  }
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[12], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[13], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.dist2 = high16;
  }
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[14], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[15], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.dist3 = high16;
  }
  measurement.apd = buffer[16];
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[17], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[18], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.volt = high16;
  }
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[19], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[20], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.temp = high16;
  }
//  memcpy(&dist, &buffer[11], 1);
//  memcpy(&dist + 1, &buffer[10], 1);
//  std::cout << std::hex << (short)buffer[11] << " " << (short)buffer[10] << " " << std::dec << dist << std::endl;
//  if (dist > 18000) {
//    return false;
//  }
  measurement.ts = timer_.elapsed();
  return true;
}

bool Driver::ReadMeasurement(Measurement &measurement) {
  std::string buffer;
  return ReadMeasurement(measurement, buffer);
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> port) {
  serial_port_ = port;
}

std::string Driver::GetVersion() {
  if (!serial_port_) {
    return std::string();
  }
  std::string buffer = Head() + std::string(1, 4) + std::string(1, 1);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  serial_port_->WriteBuffer(AppendCheckSum(buffer));
  int cnt = 0;
  std::string echo;
  while (!serial_port_->ReadBuffer(echo)) {
    if (++cnt > 10) {
      return std::string();
    }
  }
  if (echo.size() < 6) {
    return std::string();
  }
  if (echo[0] != kHead) {
    return std::string();
  }
  if (echo[1] != 7)  {
    return std::string();
  }
  if (echo[2] != 1) {
    return std::string();
  }
  std::string version;
  version =
      utils::ToDecimalString(echo[5]) + "." +
      utils::ToDecimalString(echo[4]) + "." +
      utils::ToDecimalString(echo[3]);
  return version;
}

std::string Driver::Head() {
  return std::string(1, kHead);
}

std::string Driver::AppendCheckSum(const std::string &buffer) {
  int sum = 0;
  for (auto& c : buffer) {
    sum += c;
  }
  sum &= 0xFF;
  return buffer + std::string(1, (char)sum);
}
#endif
}
