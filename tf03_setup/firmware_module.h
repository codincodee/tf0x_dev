#ifndef FIRMWARE_MODULE_H
#define FIRMWARE_MODULE_H

#include <QString>
#include <queue>
#include <memory>

enum class FirmwareUpdateStatus;
class Driver;

class FirmwareModule
{
 public:
  FirmwareModule();
  void SetDriver(std::shared_ptr<Driver> driver);
  bool LoadBin(const QString& path);
  int SegmentNum();
  bool Step();
  bool Step(const FirmwareUpdateStatus &status);
  float GetProgress();
 private:
  // int segment_length_ = 249;
  int segment_length_ = 249;
  std::queue<QByteArray> queue_;
  std::shared_ptr<Driver> driver_;
  int segment_id_ = 1;
  int segment_total_;
  int bin_bytes_;
  // QByteArray last_segment_;
};

///////////////////////////////////////////////////////////////////

#include "command_echo_widgets.h"

class QProgressBar;

struct UpgradeFirmwareWidgets : public CommandEchoWidgets {
  UpgradeFirmwareWidgets();
  void ButtonClicked() override;
  void Update() override;
  void SetOptionLingual() override;
  QComboBox* combo;
  QPushButton* browse;
  QProgressBar* progress;
  const Lingual kUpgradeLingual = {"Upgrade", "升级"};
  const Lingual kStopLingual = {"Stop", "停止"};
  const Lingual kSelectBinaryLingual =
      {"Select Firmware File", "选择固件文件"};
  const Lingual kBinaryReady = {"Firmware File Ready", "固件文件就绪"};
  const Lingual kMsgBoxErrorTitle = {"Error", "错误"};
  const Lingual kMsgBoxAbort = {"Abort", "放弃"};
  const Lingual kSerial = {"Serial Port", "串口"};
  const Lingual kCAN = {"CAN", "CAN"};

  QString bin_file;
  Lingual browse_lingual;
  std::shared_ptr<FirmwareModule> module;

  void HandleFailure();
  void HandleSuccess();
  void HandleStop();

  Q_OBJECT
 public slots:
  void OnBrowsing();
};

#endif // FIRMWARE_MODULE_H
