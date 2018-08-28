#include "command_echo_widgets.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "driver.h"
#include <QDebug>
#include "command_echo_handler.h"
#include <QLineEdit>
#include <QSizePolicy>

////////////////////// CommandEchoWidgets /////////////////////////////

CommandEchoWidgets::CommandEchoWidgets() : timeout(1000) {
  id = 0;
  item = new QLabel;
  option = nullptr;
  button = new QPushButton;
  status = new QLabel;
  button_lingual = kButtonSetLingual;
  button->setText(which_lingual(kButtonSetLingual));
  connect(button, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
}

void CommandEchoWidgets::SetOptionLingual() {

}

void CommandEchoWidgets::Update() {
  if (button->isEnabled()) {
    return;
  }
  if (timer.elapsed() > timeout) {
    button->setDisabled(false);
    status->setText(which_lingual(kNoResponseLingual));
    status_lingual = kNoResponseLingual;
  }
  if (echo_handler->IsCommandEchoed(id)) {
    button->setDisabled(false);
    if (echo_handler->IsCommandSucceeded(id)) {
      status_lingual = kSuccessLingual;
    } else if (echo_handler->IsCommandSucceeded(id)) {
      status_lingual = kFailLingual;
    } else {
      status_lingual = kUnknownLingual;
    }
    status->setText(which_lingual(status_lingual));
  }
}

void CommandEchoWidgets::OnButtonClicked() {
  return ButtonClicked();
}
void CommandEchoWidgets::ButtonClicked() {
  button->setDisabled(true);
  status->clear();
  timer.restart();
}

//void CommandEchoWidgets::SetWidgetNotApplicable(QWidget*& widget) {
//  if (widget) {
//    delete widget;
//  }
//  auto label = new QLabel;
//  label->setText("---");
//  label->setAlignment(Qt::AlignHCenter);
//  widget = label;
//}

const QString CommandEchoWidgets::kUINullString(" --- ");

QLabel* CommandEchoWidgets::UINullLabel() {
  auto label = new QLabel;
  label->setText(kUINullString);
  label->setAlignment(Qt::AlignHCenter);
  return label;
}

void CommandEchoWidgets::SetWidgetUINullLabel(QWidget *&widget) {
  if (widget) {
    delete widget;
  }
  widget = UINullLabel();
}

void CommandEchoWidgets::SetStatusLabelUINull() {
  if (status) {
    delete status;
  }
  status = UINullLabel();
  status_lingual = {kUINullString, kUINullString};
}

void CommandEchoWidgets::SetOptionWidgetUINull() {
  SetWidgetUINullLabel(option);
}

////////////////////// SetProtocolWidgets /////////////////////////////

SetProtocolWidgets::SetProtocolWidgets() : CommandEchoWidgets() {
  id = 0x44;
  item_lingual = {"Protocol", "通信协议"};
  combo = new QComboBox;
  option = combo;
}

void SetProtocolWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kDevelLingual));
  combo->addItem(which_lingual(kReleaseLingual));
}

void SetProtocolWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  if (lingual_equal(combo->currentText(), kDevelLingual)) {
    driver->SetDevelMode();
  } else if (lingual_equal(combo->currentText(), kReleaseLingual)) {
    driver->SetReleaseMode();
  } else {
    qDebug() << "Error: " << __FUNCTION__ << __LINE__;
  }
}

////////////////////// SetFrequencyWidgets /////////////////////////////

SetFrequencyWidgets::SetFrequencyWidgets() : CommandEchoWidgets() {
  id = 0x03;
  combo = new QComboBox;
  combo->addItem("1");
  combo->addItem("5");
  combo->addItem("10");
  combo->addItem("20");
  combo->addItem("30");
  combo->addItem("40");
  combo->addItem("50");
  combo->addItem("100");
  combo->addItem("500");
  combo->addItem("1000");
  item_lingual = {"Frequency (Hz)", "频率 (赫兹)"};
  option = combo;
}

void SetFrequencyWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto freq = combo->currentText().toInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetFrequency(freq);
}

void SetFrequencyWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsFrequencyEchoed()) {
    auto str = QString::number(echo_handler->Frequency());
    status_lingual = kSuccessLingual;
    status_lingual.eng += ": " + str + "Hz";
    status_lingual.chn += ": " + str + "赫兹";
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
}

////////////////////// SerialNumberWidgets /////////////////////////////

SerialNumberWidgets::SerialNumberWidgets() {
  id = 0x56;
  label = new QLabel;
  option = label;
  item_lingual = {"Serial Number", "序列号"};
  button_lingual = kButtonRequestLingual;
  button->setText(which_lingual(kButtonRequestLingual));
}

void SerialNumberWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  label->clear();
  driver->RequestSerialNumber();
}

void SerialNumberWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsSerialNumberEchoed()) {
    auto sn = echo_handler->SerialNumber();
    if (sn.isEmpty()) {
      status_lingual = kFailLingual;
    } else {
      status_lingual = kSuccessLingual;
    }
    status->setText(which_lingual(status_lingual));
    label->setText(sn);
    button->setDisabled(false);
  }
}

////////////////////// OutputSwitchWidgets /////////////////////////////

OutputSwitchWidgets::OutputSwitchWidgets() {
  id = 0x07;
  combo = new QComboBox;
  option = combo;
  item_lingual = {"Output Switch", "输出开关"};
}

void OutputSwitchWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  if (lingual_equal(combo->currentText(), kSwitchOnLingual)) {
    driver->SetOutputSwitchOn();
  } else if (lingual_equal(combo->currentText(), kSwitchOffLingual)) {
    driver->SetOutputSwitchOff();
  }
}

void OutputSwitchWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsOutputSwitchEchoed()) {
    if (echo_handler->IsOutputOn()) {
      status_lingual = kOutputOnLingual;
    } else {
      status_lingual = kOutputOffLingual;
    }
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
//  if (echo_handler->IsSerialNumberEchoed()) {
//    auto sn = echo_handler->SerialNumber();
//    if (sn.isEmpty()) {
//      status_lingual = kFailLingual;
//    } else {
//      status_lingual = kSuccessLingual;
//    }
//    status->setText(which_lingual(status_lingual));
//    label->setText(sn);
//    button->setDisabled(false);
//  }
}

void OutputSwitchWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kSwitchOnLingual));
  combo->addItem(which_lingual(kSwitchOffLingual));
}

////////////////////// MeasureTriggerWidgets /////////////////////////////

MeasureTriggerWidgets::MeasureTriggerWidgets() {
  id = 0x04;
  item_lingual = {"Trigger Once", "单次触发"};
  button_lingual = {"Trigger", "触发"};
  SetOptionWidgetUINull();
  SetStatusLabelUINull();
}

void MeasureTriggerWidgets::ButtonClicked() {
  driver->TriggerOnce();
}

void MeasureTriggerWidgets::Update() {

}

////////////////////// FlashSettingsWidgets /////////////////////////////

FlashSettingsWidgets::FlashSettingsWidgets() {
  id = 0x11;
  timeout = 3000;
  item_lingual = {"Flash Settings", "写入设置"};
  SetOptionWidgetUINull();
}

void FlashSettingsWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  driver->SaveSettingsToFlash();
}

////////////////////// RestoreFactoryWidgets /////////////////////////////

RestoreFactoryWidgets::RestoreFactoryWidgets() {
  id = 0x10;
  timeout = 3000;
  item_lingual = {"Restore Factory", "出厂设置"};
  SetWidgetUINullLabel(option);
}

void RestoreFactoryWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  driver->RestoreFactory();
}

////////////////////// SetSerialBaudRateWidgets /////////////////////////////

SetSerialBaudRateWidgets::SetSerialBaudRateWidgets() {
  id = 0x06;
  item_lingual = {"Baud Rate", "设置波特率"};
  combo = new QComboBox;
  auto baud_rates = Driver::BaudRates();
  for (auto& rate : baud_rates) {
    combo->addItem(QString::number(rate));
  }
  option = combo;
}

void SetSerialBaudRateWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto rate = combo->currentText().toInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetDeviceBaudRate(rate);
}

void SetSerialBaudRateWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsBaudRateEchoed()) {
    auto rate = echo_handler->BaudRate();
    auto str = QString::number(rate);
    status_lingual = kSuccessLingual;
    status_lingual.eng = status_lingual.eng + ": " + str;
    status_lingual.chn = status_lingual.chn + ": " + str;
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
}

////////////////////// SetPortTypeWidgets /////////////////////////////

SetPortTypeWidgets::SetPortTypeWidgets() {
  id = 0x45;
  item_lingual = {"Port Type", "传输类型"};
  combo = new QComboBox;
  SetOptionLingual();
  option = combo;
}

void SetPortTypeWidgets::SetOptionLingual() {
  CommandEchoWidgets::SetOptionLingual();
  combo->clear();
  combo->addItem(which_lingual(kSerial));
  combo->addItem(which_lingual(kCAN));
}

void SetPortTypeWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto type = combo->currentText();
  if (lingual_equal(type, kSerial)) {
    driver->SetTransTypeSerial();
  } else if (lingual_equal(type, kCAN)) {
    driver->SetTransTypeCAN();
  }
}

////////////////////// SetOutputFormatWidgets /////////////////////////////

SetOutputFormatWidgets::SetOutputFormatWidgets() {
  id = 0x05;
  // timeout = 3000;
  item_lingual = {"Output Format", "输出格式"};
  combo = new QComboBox;
  option = combo;
}

void SetOutputFormatWidgets::SetOptionLingual() {
  CommandEchoWidgets::SetOptionLingual();
  combo->clear();
  combo->addItem(which_lingual(kNineBytes));
  combo->addItem(which_lingual(kPixhawk));
}

void SetOutputFormatWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto type = combo->currentText();
  if (lingual_equal(type, kNineBytes)) {
    driver->SetOutputFormatNineBytes();
  } else if (lingual_equal(type, kPixhawk)) {
    driver->SetOutputFormatPIX();
  }
}

void SetOutputFormatWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsOutputFormatEchoed()) {
    auto format = echo_handler->GetOutputFormat();
    status_lingual = kSuccessLingual;
    if (format == OutputFormat::nine_bytes) {
      status_lingual = status_lingual + ": " + kNineBytes;
    } else if (format == OutputFormat::pix) {
      status_lingual = status_lingual + ": " + kPixhawk;
    } else {
      return;
    }
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
}

////////////////////// SetCANIDWidgetsBase /////////////////////////////

SetCANIDWidgetsBase::SetCANIDWidgetsBase() {
  edit = new QLineEdit;
  edit->setValidator(new QIntValidator(0, 100000, this));
  option = edit;
}

////////////////////// SetCANSendIDWidgets /////////////////////////////

SetCANSendIDWidgets::SetCANSendIDWidgets() {
  id = 0x50;
  item_lingual = {"CAN Send ID", "CAN发送ID"};
}

void SetCANSendIDWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto id = edit->text().toUInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetCANSendID(id);
}

////////////////////// SetCANReceiveIDWidgets /////////////////////////////

SetCANReceiveIDWidgets::SetCANReceiveIDWidgets() {
  id = 0x51;
  item_lingual = {"CAN Receive ID", "CAN接收ID"};
}

void SetCANReceiveIDWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto id = edit->text().toUInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetCANReceiveID(id);
}
