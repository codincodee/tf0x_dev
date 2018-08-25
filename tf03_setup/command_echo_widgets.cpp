#include "command_echo_widgets.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "driver.h"
#include <QDebug>

////////////////////// CommandEchoWidgets /////////////////////////////

CommandEchoWidgets::CommandEchoWidgets() {
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
}

void CommandEchoWidgets::OnButtonClicked() {
  return ButtonClicked();
}
void CommandEchoWidgets::ButtonClicked() {}

////////////////////// SetProtocolWidgets /////////////////////////////

SetProtocolWidgets::SetProtocolWidgets() : CommandEchoWidgets() {
  item_lingual = {"Protocol", "通信协议"};
  combo = new QComboBox;
  option = combo;
}

void SetProtocolWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(devel));
  combo->addItem(which_lingual(release));
}

void SetProtocolWidgets::ButtonClicked() {
  button->setDisabled(true);
  status->clear();
  timer.restart();
  if (lingual_equal(combo->currentText(), devel)) {
    driver->SetDevelMode();
  } else if (lingual_equal(combo->currentText(), release)) {
    driver->SetReleaseMode();
  } else {
    qDebug() << "Error: " << __FUNCTION__ << __LINE__;
  }
}
