#include "command_echo_widgets.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "driver.h"
#include <QDebug>
#include "command_echo_handler.h"

////////////////////// CommandEchoWidgets /////////////////////////////

CommandEchoWidgets::CommandEchoWidgets() {
  id = 0;
  item = new QLabel;
  button = new QPushButton;
  status = new QLabel;
  button_lingual = kButtonLingual;
  button->setText(which_lingual(kButtonLingual));
  connect(button, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
}

void CommandEchoWidgets::SetOptionLingual() {

}

void CommandEchoWidgets::Update() {
  if (button->isEnabled()) {
    return;
  }
  if (timer.elapsed() > 1000) {
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
