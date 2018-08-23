#ifndef DRIVER_H
#define DRIVER_H

#include <memory>
#include <thread>
#include <vector>
#include <functional>
#include "parsed.h"

class QSerialPort;
class QByteArray;

enum class ParsedType {
  measure, status
};

struct Parsed {
  ParsedType type;
  std::unique_ptr<ParsedBase> data;
};

class Driver
{
 public:
  // Configurations

  // Initialize
  bool Open();
  bool Close();

 private:
  void WorkThread();

  std::thread work_thead_;
  bool stop_signal_;

  // In work thread only
  void HandleIncomingCommandInWorkThread();
  void ProcessBufferInWorkThread(QByteArray& buffer);
  QByteArray buffer_;
  std::shared_ptr<QSerialPort> serial_port_;

  // Parsers
  static bool ParseNineByteMeasure(
      const QByteArray& buffer, Parsed& parsed, int& from, int& to);
  static QByteArray ParseNineByteMeasureMessageAtFront(
      const QByteArray& buffer, int& from, int& to);
  static bool CheckSum(const QByteArray& buffer, const int& from, const int& to);
  using ReceiveParser =
      std::function<bool(
          const QByteArray& buffer, Parsed& parsed, int& from, int& to)>;
  std::vector<ReceiveParser> receive_parsers_;
  std::vector<Parsed> receive_commands_;
  Parsed latest_measure_;

  void LoadAllParsers(std::vector<ReceiveParser>& parsers);
};

#endif // DRIVER_H
