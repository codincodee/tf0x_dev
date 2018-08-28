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

void Driver::SetFrequency(const unsigned short &frequency) {
  EnqueueCommand([this, frequency](){
    return SendMessage(CommonCommand(char(0x03), to_bytes(frequency)));
  });
}

void Driver::RequestSerialNumber() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x56), QByteArray(1, 0x00)));
  });
}

void Driver::SetOutputSwitchOn() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x07), QByteArray(1, 0x01)));
  });
}

void Driver::SetOutputSwitchOff() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x07), QByteArray(1, 0x00)));
  });
}

void Driver::TriggerOnce() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x04), QByteArray()));
  });
}

void Driver::SaveSettingsToFlash() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x11), QByteArray()));
  });
}

void Driver::RestoreFactory() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x10), QByteArray()));
  });
}

void Driver::SetDeviceBaudRate(const uint32_t &rate) {
  EnqueueCommand([this, rate](){
    return SendMessage(CommonCommand(char(0x06), to_bytes(rate)));
  });
}

void Driver::SetTransTypeCAN() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x45), QByteArray(1, 0x02)));
  });
}

void Driver::SetOutputFormatNineBytes() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x05), QByteArray(1, 0x01)));
  });
}

void Driver::SetOutputFormatPIX() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x05), QByteArray(1, 0x02)));
  });
}

void Driver::SetCANSendID(const uint32_t &id) {
  EnqueueCommand([this, id](){
    return SendMessage(CommonCommand(char(0x50), to_bytes(id)));
  });
}

void Driver::SetCANReceiveID(const uint32_t &id) {
  EnqueueCommand([this, id](){
    return SendMessage(CommonCommand(char(0x51), to_bytes(id)));
  });
}

void Driver::SetDeviceCANBaudRate(const uint32_t &rate) {
  EnqueueCommand([this, rate](){
    return SendMessage(CommonCommand(char(0x52), to_bytes(rate)));
  });
}

void Driver::SetOutRangeValue(const uint16_t &value) {
  EnqueueCommand([this, value](){
    return SendMessage(CommonCommand(char(0x4f), to_bytes(value)));
  });
}

void Driver::SetTransTypeSerial() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x45), QByteArray(1, 0x01)));
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
