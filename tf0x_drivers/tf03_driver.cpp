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
  dist0 = measurement.dist1;
  return true;
}

bool Driver::ReadMeasurement(Measurement& measurement, std::string& buffer) {
  return ReadMeasurement(measurement);
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
  measurement.dist1 = dist * 100;
  measurement.ts = timer_.elapsed();
  return true;
}

void Driver::SetSerialPort(
    std::shared_ptr<tf0x_driver::AbstractSerialPort> port) {
  serial_port_ = port;
}

bool Driver::ResetDevice() {
  return false;
}

bool Driver::RestoreFactory() {
  return false;
}

bool Driver::SetVdbs(const uint16_t &value) {
  return false;
}
bool Driver::SaveSettings() {
  return false;
}

bool Driver::SetAPD(const uint8_t &value) {
  return false;
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
  timer_.start();
  return true;
}

bool Driver::ReadDistance(double &dist0) {
//  Measurement measurement;
//  if (!ReadMeasurement(measurement)) {
//    return false;
//  }
//  dist0 = measurement.dist1;
  return true;
}

std::vector<Measurement> Driver::ReadMeasurements(std::string& buffer) {
  std::vector<Measurement> empty;
  if (!serial_port_) {
    return empty;
  }
  if (!serial_port_->ReadBuffer(buffer)) {
    return empty;
  }
  if (buffer.empty()) {
    return empty;
  }

  std::vector<Measurement> results;
  buffer_ += buffer;
  while (!buffer_.empty()) {
    for (int i = 0; i < buffer_.size(); ++i) {
      if (buffer_[i] == kHead) {
        if (i != 0) {
          buffer_.erase(buffer_.begin(), buffer_.begin() + i);
        }
        break;
      }
    }

    if (buffer_[0] == kHead) {
      if (buffer_.size() >= 22) {
        std::string single;
        single.assign(buffer_.begin(), buffer_.begin() + 22);
        if (IsValidBuffer(single)) {
          results.push_back(ParseBuffer(single));
          buffer_.erase(buffer_.begin(), buffer_.begin() + 22);
        } else {
          if (!DetectAndHandleEcho()) {
            buffer_.erase(buffer_.begin());
          }
        }
      } else {
        break;
      }
    } else {
      break;
    }
  }
  std::vector<Measurement> results_reverse;
  results_reverse.reserve(results.size());
  for (auto& i = results.rbegin(); i != results.rend(); ++i) {
    results_reverse.push_back(*i);
  }
  return results_reverse;
}

bool Driver::DetectAndHandleEcho() {
  if (buffer_.size() < 2) {
    return false;
  }
  int len = buffer_[1];
  if (buffer_.size() < len) {
    return false;
  }
  std::string buf(buffer_.begin(), buffer_.begin() + len);
  if (!IsValidEchoBuffer(buf)) {
    return false;
  }
  switch (buf[2]) {
  case 0x40:
  {
    if (buf[3] == 0) {
      set_apd_echo.push_back({true});
      std::cout << __LINE__ << std::endl;
    } else {
      set_apd_echo.push_back({false});
      std::cout << __LINE__ << std::endl;
    }
  } break;
  }
  if (len <= 0) {
    return false;
  }
  buffer_.erase(buffer_.begin(), buffer_.begin() + len);
  return true;
}
//bool Driver::ReadMeasurement(Measurement &measurement) {
//  std::string buffer;
//  return ReadMeasurement(measurement, buffer);
//}

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

bool Driver::ResetDevice() {
  std::string buffer = Head() + std::string(1, 4) + std::string(1, 2);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(AppendCheckSum(buffer))) {
    return false;
  }
  return true;
}

bool Driver::SetFrequency(const uint16_t &value) {
  if (!serial_port_) {
    return false;
  }
  char vc[2];
  memcpy(vc, &value, 2);
  std::string buffer = Head() + std::string(1, 6) + std::string(1, 0x03) + std::string(1, vc[0]) + std::string(1, vc[1]);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

std::string Driver::Head() {
  return std::string(1, kHead);
}

bool Driver::IsValidBuffer(const std::string& buffer) {
  if (buffer.size() < 22) {
    return false;
  }
  int sum = 0;
  for (int i = 0; i < 21; ++i) {
    sum += buffer[i];
  }
  sum &= 0xFF;
  char check = sum;
  return check == buffer[21];
}

Measurement Driver::ParseBuffer(const std::string &buffer) {
  Measurement measurement;
  if (buffer.size() < 22) {
    return measurement;
  }
  measurement.ts = timer_.elapsed();
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[2], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[3], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.algorithm = high16;
  }
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[4], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[5], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.raw_dist1 = high16;
  }
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[6], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[7], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.raw_dist2 = high16;
  }
  {
    unsigned char low = 0x00;
    memcpy(&low, &buffer[8], 1);

    unsigned char high = 0x00;
    memcpy(&high, &buffer[9], 1);
    uint16_t high16 = high;
    high16 = high16 << 8;
    high16 |= low;

    measurement.raw_dist3 = high16;
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

    measurement.temp = (high16 * 3300 / 4096 - 760) / 2.5 + 25;
  }
  return measurement;
}

std::string Driver::AppendCheckSum(const std::string &buffer) {
  int sum = 0;
  for (auto& c : buffer) {
    sum += c;
  }
  sum &= 0xFF;
  return buffer + std::string(1, (char)sum);
}

bool Driver::SetAPD(const uint8_t &value) {
  if (!serial_port_) {
    return false;
  }
  char vc;
  memcpy(&vc, &value, 1);
  std::string buffer = Head() + std::string(1, 5) + std::string(1, 0x40) + std::string(1, vc);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

bool Driver::RestoreFactory() {
  if (!serial_port_) {
    return false;
  }
  std::string buffer = Head() + std::string(1, 4) + std::string(1, 0x10);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

bool Driver::SaveSettings() {
  if (!serial_port_) {
    return false;
  }
  std::string buffer = Head() + std::string(1, 4) + std::string(1, 0x11);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

bool Driver::SetVdbs(const uint16_t &value) {
  if (!serial_port_) {
    return false;
  }
  char vc[2];
  memcpy(vc, &value, 2);
  std::string buffer = Head() + std::string(1, 6) + std::string(1, 0x41) + std::string(1, vc[0]) + std::string(1, vc[1]);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

bool Driver::SetTableCorrA(const double &value) {
  if (!serial_port_) {
    return false;
  }
  int16_t value_us = value * 1000;
  char vc[2];
  memcpy(vc, &value_us, 2);
  std::string buffer = Head() + std::string(1, 6) + std::string(1, 0x42) + std::string(1, vc[0]) + std::string(1, vc[1]);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

bool Driver::SetTableCorrB(const int16_t &value) {
  if (!serial_port_) {
    return false;
  }
  char vc[2];
  memcpy(vc, &value, 2);
  std::string buffer = Head() + std::string(1, 6) + std::string(1, 0x43) + std::string(1, vc[0]) + std::string(1, vc[1]);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

bool Driver::SetProtocolType(const ProtocolType &type) {
  if (!serial_port_) {
    return false;
  }
  char type_code;
  switch (type) {
  case ProtocolType::release: type_code = 1; break;
  case ProtocolType::develop: type_code = 2; break;
  default: return false;
  }
  std::string buffer = Head() + std::string(1, 5) + std::string(1, 0x44) + std::string(1, type_code);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}

bool Driver::SetTransType(const TransType &type) {
  if (!serial_port_) {
    return false;
  }
  char type_code;
  switch (type) {
  case TransType::serial: type_code = 1; break;
  case TransType::can: type_code = 2; break;
  default: return false;
  }
  std::string buffer = Head() + std::string(1, 5) + std::string(1, 0x45) + std::string(1, type_code);
  auto cmd = AppendCheckSum(buffer);
  std::string recycle;
  serial_port_->ReadBuffer(recycle);
  if (!serial_port_->WriteBuffer(cmd)) {
    return false;
  }
  return true;
}
#endif
}
