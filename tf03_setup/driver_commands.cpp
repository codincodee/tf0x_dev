#include "driver.h"

void Driver::EnqueueCommand(const CommandFunc &command) {
  command_queue_mutex_.lock();
  command_queue_.push(command);
  command_queue_mutex_.unlock();
}

void Driver::SetDevelMode() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x44), QByteArray(1, 2)));
  });
}

void Driver::SetReleaseMode() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x44), QByteArray(1, 1)));
  });
}

std::vector<Message> Driver::GetMessages() {
  receive_messages_mutex_.lock();
  auto messages = std::move(receive_messages_);
  receive_messages_mutex_.unlock();
  return messages;
}

QByteArray Driver::CommonCommand(const char &id, const QByteArray &data) {
  QByteArray msg;
  msg += 0x5a;
  msg += data.size() + 4;
  msg += id;
  msg += data;
  msg += CalculateSum(msg);
  return msg;
}

QByteArray Driver::CalculateSum(const QByteArray &msg) {
  char sum = 0;
  for (auto& b : msg) {
    sum += b;
  }
  return QByteArray(1, sum);
}
