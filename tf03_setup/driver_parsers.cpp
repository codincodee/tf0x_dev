#include "driver.h"
#include <QDebug>
#include <iostream>

void Driver::LoadAllParsers(std::vector<ReceiveParser> &parsers) {
  parsers.clear();
  parsers.push_back(
      std::bind(
          Driver::ParseStatusEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseFrequencyEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          Driver::ParseSerialNumberEcho,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
  parsers.push_back(
      std::bind(
          &Driver::ParseNineByteMeasure,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
}

bool Driver::ParseFrequencyEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 6) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x03)) {
    return false;
  }
  parsed.type = MessageType::frequency;
  std::unique_ptr<FrequencyEcho> data(new FrequencyEcho);
  memcpy(&data->frequency, msg.data() + 3, 2);
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseSerialNumberEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() < 6) {
    return false;
  }
  auto id = msg[2];
  if (id != char(0x56)) {
    return false;
  }
  parsed.type = MessageType::serial_number;
  std::unique_ptr<SerialNumberEcho> data(new SerialNumberEcho);
  auto len = msg[1] - 5;
  data->sn = QString(len, 0);
  data->status = (msg[3] == char(0x00));
  memcpy(data->sn.data(), msg.data() + 4, len);
  parsed.data = std::move(data);
  return true;
}

bool Driver::ParseStatusEcho(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = Parse0x5AMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  if (msg.size() != 5)  {
    return false;
  }
  auto id = msg[2];
  if (kEchoStatusIDMap.count(id) == 0) {
    return false;
  }
  parsed.type = MessageType::status;
  std::unique_ptr<StatusEcho> data(new StatusEcho);
  data->command = kEchoStatusIDMap[id];
  data->success = (msg.at(3) == 0);
  data->cmd_id = id;
  parsed.data = std::move(data);
  return true;
}

QByteArray Driver::Parse0x5AMessageAtFront(
    const QByteArray &buffer, int &from, int &to) {
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == 0x5A) {
      auto ii = i + 1;
      if (ii < buffer.size()) {
        auto len = buffer[ii];
        auto end = ii + len - 2;
        if (end < buffer.size()) {
          if (CheckSum(buffer, i, end)) {
            from = i;
            to = end;
            result.reserve(to - from + 1);
            for (int j = from; j <= to; ++j) {
              result.push_back(buffer[j]);
            }
            break;
          }
        }
      }
    }
  }
  return result;
}

bool Driver::ParseNineByteMeasure(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = ParseNineByteMeasureMessageAtFront(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  std::unique_ptr<MeasureBasic> measure(new MeasureBasic);
  ++measure_id_;
  measure->id = measure_id_;
  memcpy(&measure->dist, msg.data() + 2, 2);
  memcpy(&measure->amp, msg.data() + 4, 2);
  parsed.type = MessageType::measure;
  parsed.data = std::move(measure);
  return true;
}

QByteArray Driver::ParseNineByteMeasureMessageAtFront(
    const QByteArray &buffer, int &from, int &to) {
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == 0x59) {
      auto ii = i + 1;
      if (ii < buffer.size()) {
        if (buffer[ii] == 0x59) {
          auto end = ii + 7;
          if (end < buffer.size()) {
            if (CheckSum(buffer, i, end)) {
              from = i;
              to = end;
              result.reserve(to - from + 1);
              for (int j = from; j <= to; ++j) {
                result.push_back(buffer[j]);
              }
              break;
            }
          }
        }
      }
    }
  }
  return result;
}

bool Driver::CheckSum(const QByteArray &buffer, const int &from, const int &to) {
  char sum = 0;
  for (int i = from; i < to; ++i) {
    sum += buffer[i];
  }
  return buffer[to] == sum;
}
