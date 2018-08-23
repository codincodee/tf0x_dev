#include "driver.h"
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QElapsedTimer>
#include "static_unique_ptr_cast.h"

bool Driver::Open() {
  LoadAllParsers(receive_parsers_);
  stop_signal_ = false;
  work_thead_ = std::thread(&Driver::WorkThread, this);
  return true;
}

bool Driver::Close() {
  stop_signal_ = true;
  if (work_thead_.joinable()) {
    work_thead_.join();
  }
  return true;
}

void Driver::WorkThread() {
  serial_port_.reset(new QSerialPort);
  serial_port_->setBaudRate(115200);
  auto ports = QSerialPortInfo::availablePorts();
  if (ports.empty()) {
    return;
  }
  serial_port_->setPortName(ports.begin()->portName());
  if (!serial_port_->open(QIODevice::ReadWrite)) {
    return;
  }

  while (!stop_signal_) {
    HandleIncomingCommandInWorkThread();
    if (serial_port_->waitForReadyRead(100)) {
      buffer_ += serial_port_->readAll();
      ProcessBufferInWorkThread(buffer_);
    }
  }
  serial_port_->close();
}

void Driver::HandleIncomingCommandInWorkThread() {

}

void Driver::ProcessBufferInWorkThread(QByteArray &buffer) {
//  if (buffer.size() > 20) {
//    static int cnt  = 0;
//    qDebug() << "final: " << buffer.size() << ++cnt;
//    // exit(0);
//  }
//  qDebug() << "buffer: " << buffer;
  Parsed parsed;
  while (true) {
    int from = 0, to = 0;
    int parsed_cnt = 0;
    for (auto& parser : receive_parsers_) {
      if (parser(buffer, parsed, from, to)) {
        ++parsed_cnt;
//        qDebug() << to;
        buffer = buffer.remove(0, to + 1);
        if (parsed.type == ParsedType::measure) {
          latest_measure_ = std::move(parsed);
          std::unique_ptr<MeasureBasic> measure = static_unique_ptr_cast<MeasureBasic>(std::move(latest_measure_.data));
          if (measure) {
            qDebug() << measure->dist;
          }
        } else {
          receive_commands_.emplace_back(std::move(parsed));
          // qDebug() << __LINE__;
        }
      } else {
//        static int cnt = 0;
//        qDebug() << __FUNCTION__ << (cnt++) << buffer;
      }
    }
    if (parsed_cnt == 0) {
      break;
    }
  }
//      qDebug() << buffer;
}
