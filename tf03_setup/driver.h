#ifndef DRIVER_H
#define DRIVER_H

#include <memory>
#include <thread>
#include <vector>
#include <functional>
#include "parsed.h"
#include <mutex>
#include <queue>
#include <unordered_map>
#include "lingual.h"

class QSerialPort;
class QByteArray;

QByteArray to_bytes(const unsigned short& value);

enum class MessageType {
  measure, status, frequency, serial_number
};

struct Message {
  MessageType type;
  std::unique_ptr<ParsedBase> data;
};

class Driver
{
 public:
  Driver();
  // Configurations

  // Initialize
  bool Open();
  bool Close();
  bool LastMeasure(MeasureBasic& measure);
  void SetDevelMode();
  void SetReleaseMode();
  void SetFrequency(const unsigned short& frequency);
  void RequestSerialNumber();
  std::vector<Message> GetMessages();

 private:
  using CommandFunc = std::function<bool()>;

  bool SendMessage(const QByteArray& msg);
  void EnqueueCommand(const CommandFunc& command);
  QByteArray CommonCommand(const char& id, const QByteArray& data);
  QByteArray CalculateSum(const QByteArray& msg);
  void WorkThread();

  std::thread work_thead_;
  bool stop_signal_;

  // In work thread only
  void HandleIncomingCommandInWorkThread();
  void ProcessBufferInWorkThread(QByteArray& buffer);
  QByteArray buffer_;
  std::shared_ptr<QSerialPort> serial_port_;
  unsigned long measure_id_ = 0;

  // Parsers
  bool ParseNineByteMeasure(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static QByteArray Parse0x5AMessageAtFront(
      const QByteArray& buffer, int& from, int& to);
  static bool ParseStatusEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseFrequencyEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static bool ParseSerialNumberEcho(
      const QByteArray& buffer, Message& parsed, int& from, int& to);
  static QByteArray ParseNineByteMeasureMessageAtFront(
      const QByteArray& buffer, int& from, int& to);
  static bool CheckSum(const QByteArray& buffer, const int& from, const int& to);
  using ReceiveParser =
      std::function<bool(
          const QByteArray& buffer, Message& parsed, int& from, int& to)>;
  std::vector<ReceiveParser> receive_parsers_;

  std::mutex receive_messages_mutex_;
  std::vector<Message> receive_messages_;

  std::mutex latest_measure_mutex_;
  Message latest_measure_;

  std::mutex command_queue_mutex_;
  std::queue<CommandFunc> command_queue_;

  void LoadAllParsers(std::vector<ReceiveParser>& parsers);

  static std::unordered_map<char, Lingual> kEchoStatusIDMap;
};

#endif // DRIVER_H
