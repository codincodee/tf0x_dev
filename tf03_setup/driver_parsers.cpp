#include "driver.h"
#include <QDebug>
#include <iostream>

void Driver::LoadAllParsers(std::vector<ReceiveParser> &parsers) {
  parsers.push_back(
      std::bind(
          Driver::ParseNineByteMeasure,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
}

bool Driver::ParseNineByteMeasure(
    const QByteArray &buffer, Parsed &parsed, int &from, int &to) {
  auto msg = ParseNineByteMeasureMessageAtFront(buffer, from, to);
//   qDebug() << buffer.size() << msg;
  if (msg.isEmpty()) {
    return false;
  }
  std::unique_ptr<MeasureBasic> measure(new MeasureBasic);
  memcpy(&measure->dist, msg.data() + 2, 2);
  memcpy(&measure->amp, msg.data() + 4, 2);
  parsed.type = ParsedType::measure;
  parsed.data = std::move(measure);
  return true;
}

QByteArray Driver::ParseNineByteMeasureMessageAtFront(
    const QByteArray &buffer, int &from, int &to) {
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == 0x59) {
//      qDebug() << __LINE__;
      auto ii = i + 1;
      if (ii < buffer.size()) {
        if (buffer[ii] == 0x59) {
//          qDebug() << __LINE__;
          auto end = ii + 7;
          if (end < buffer.size()) {
//            qDebug() << __LINE__ << i << end;
            if (CheckSum(buffer, i, end)) {
//              qDebug() << __LINE__;
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
//  sum &= 0xff;

  return buffer[to] == sum;
}
