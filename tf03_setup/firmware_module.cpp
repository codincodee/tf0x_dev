#include "firmware_module.h"
#include <QFile>
#include <QDebug>
#include "parsed.h"
#include "driver.h"
#include <QProgressBar>
#include <QPushButton>
#include <QMessageBox>
#include "command_echo_handler.h"
#include <QLabel>
#include <QFileDialog>
#include <QComboBox>

FirmwareModule::FirmwareModule()
{

}

void FirmwareModule::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

bool FirmwareModule::LoadBin(const QString &path) {
  QFile file(path);
  file.open(QIODevice::ReadOnly);
  auto stream = file.readAll();
  file.close();
  if (stream.isEmpty()) {
    return false;
  }
  while (!queue_.empty()) {
    queue_.pop();
  }
  int index = 0;
  while (index < stream.size()) {
    auto end = index + segment_length_;
    if (end > stream.size()) {
      end = stream.size();
    }
    QByteArray seg;
    seg.reserve(segment_length_);
    for (int i = index; i < end; ++i){
      seg.push_back(stream[i]);
    }
    queue_.push(seg);
    index += segment_length_;
  }
  segment_total_ = queue_.size();
  bin_bytes_ = stream.size();
  return true;
}

int FirmwareModule::SegmentNum() {
  return queue_.size();
}

bool FirmwareModule::Step() {
  driver_->SendFirmwareFirstSegment(
      bin_bytes_, QByteArray(segment_length_ - 2, 0));
  return true;
}

bool FirmwareModule::Step(const FirmwareUpdateStatus &status) {
  if (status != FirmwareUpdateStatus::ok) {
    return false;
  }
  auto seg = queue_.front();
  queue_.pop();
  auto id = segment_total_ - SegmentNum();
  if (queue_.empty()) {
    seg += QByteArray(segment_length_ - seg.size(), 0);
    driver_->SendFirmwareLastSegment(id, seg);
  } else {
    driver_->SendFirmwareSegment(id, seg);
  }

  return true;
}

float FirmwareModule::GetProgress() {
  return (segment_total_ - SegmentNum()) * 1.0f / segment_total_;
}

////////////////////// UpgradeFirmwareWidgets /////////////////////////////

UpgradeFirmwareWidgets::UpgradeFirmwareWidgets() {
  id = 0x49;
  item_lingual = {"Upgrade Firmware", "升级固件"};
  button_lingual = kUpgradeLingual;
  browse = new QPushButton;
  combo = new QComboBox;
  option = combo;
//  browse->setFlat(true);
  progress = new QProgressBar;
  progress->setMinimum(0);
  progress->setMaximum(100);
  progress->setValue(0);
  progress->setVisible(false);
  status->setVisible(false);
//  DeleteStatusWidget();
//  status_base = progress;
  connect(browse, SIGNAL(clicked()), this, SLOT(OnBrowsing()));
  browse_lingual = kSelectBinaryLingual;
}

void UpgradeFirmwareWidgets::ButtonClicked() {
  if (!module && driver) {
    module.reset(new FirmwareModule);
    module->SetDriver(driver);
  }
  if (lingual_equal(button->text(), kUpgradeLingual)) {
    if (bin_file.isEmpty()) {
      return;
    }
    if (!module->LoadBin(bin_file)) {
      QMessageBox box(button);
      box.setWindowTitle(which_lingual(kMsgBoxErrorTitle));
      box.setText(
          which_lingual({"Failed to open firmware file.", "固件文件无法打开。"}));
      box.setStandardButtons(QMessageBox::Abort);
      box.setButtonText(QMessageBox::Abort, which_lingual(kMsgBoxAbort));
      box.exec();
      HandleFailure();
      return;
    }
    driver->SetOutputSwitchOff();
    driver->SetBufferCleanerBytes(1000);
    button_lingual = kStopLingual;
    button->setText(which_lingual(button_lingual));
    timer.restart();
    progress->setValue(0);
    progress->setVisible(true);
    status->setVisible(false);
    module->Step();
  } else if (lingual_equal(button->text(), kStopLingual)) {
    HandleFailure();
  }
}

void UpgradeFirmwareWidgets::Update() {
  if (!lingual_equal(button->text(), kStopLingual)) {
    return;
  }
  if (timer.elapsed() > 4000) {
    HandleFailure();
    return;
  }
  if (module->SegmentNum() == 0) {
    HandleSuccess();
    return;
  }
  if (echo_handler->IsFirmwareUpdateEchoed()) {
    if (!module->Step(echo_handler->GetFirmwareUpdateStatus())) {
      HandleFailure();
      return;
    } else {
      timer.restart();
      progress->setValue(module->GetProgress() * 100);
    }
  }
}

void UpgradeFirmwareWidgets::SetOptionLingual() {
  browse->setText(which_lingual(browse_lingual));
  combo->clear();
  combo->addItem(which_lingual(kSerial));
  combo->addItem(which_lingual(kCAN));
}

void UpgradeFirmwareWidgets::HandleStop() {
  status->setVisible(true);
  progress->setVisible(false);
  button_lingual = kUpgradeLingual;
  button->setText(which_lingual(button_lingual));
  bin_file.clear();
  browse_lingual = kSelectBinaryLingual;
  browse->setText(which_lingual(browse_lingual));
  driver->SetBufferCleanerBytesDefault();
}

void UpgradeFirmwareWidgets::HandleSuccess() {
  status_lingual = kSuccessLingual;
  status->setText(which_lingual(status_lingual));
  HandleStop();
}

void UpgradeFirmwareWidgets::HandleFailure() {
  status_lingual = kFailLingual;
  status->setText(which_lingual(status_lingual));
  HandleStop();
}

void UpgradeFirmwareWidgets::OnBrowsing() {
  auto file =
      QFileDialog::getOpenFileName(
          option, which_lingual({"Select Firmware File", "选择固件文件"}),
          QDir::homePath(), "Firmware File (*.bin)");
  if (file.isEmpty()) {
    return;
  }
  bin_file = file;
  browse_lingual = kBinaryReady;
  browse->setText(which_lingual(browse_lingual));
}
