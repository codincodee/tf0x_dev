#include "main_window.h"
#include "ui_main_window.h"
#include <QMessageBox>
#include <tf0x_common/qt_helpers.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <iostream>
#include <QThread>
#include <QTextCursor>

const QString kCommandSetProtocolReleaseProtocol = "Release";
const QString kCommandSetProtocolDevelopProtocol = "Develop";

const QString kCommandSetTransTypeSerial = "Serial";
const QString kCommandSetTransTypeCAN = "CAN";

const QString kCommandVdbsAdjustTypeDisabled = "Disabled";
const QString kCommandVdbsAdjustTypeAlgorithm = "Algorithm";
const QString kCommandVdbsAdjustTypeTDC = "TDC";

const QString kCommandAPDAutoEnable = "Enable";
const QString kCommandAPDAutoDisable = "Disable";

void MainWindow::InitializeCommandPageCartSection() {
  ui->CommandPageSetProtocolComboBox->addItem(kCommandSetProtocolReleaseProtocol);
  ui->CommandPageSetProtocolComboBox->addItem(kCommandSetProtocolDevelopProtocol);

  ui->CommandPageSetTransTypeComboBox->addItem(kCommandSetTransTypeCAN);
  ui->CommandPageSetTransTypeComboBox->addItem(kCommandSetTransTypeSerial);

  ui->CommandPageVdbsAdjustComboBox->addItem(kCommandVdbsAdjustTypeTDC);
  ui->CommandPageVdbsAdjustComboBox->addItem(kCommandVdbsAdjustTypeAlgorithm);
  ui->CommandPageVdbsAdjustComboBox->addItem(kCommandVdbsAdjustTypeDisabled);

  ui->CommandPageAPDAutoComboBox->addItem(kCommandAPDAutoEnable);
  ui->CommandPageAPDAutoComboBox->addItem(kCommandAPDAutoDisable);
  // ui->CommandPageWriteParamLineEdit->setReadOnly(true);
}

void MainWindow::on_CommandPageSetAPDPushButton_clicked()
{
  return HandleCommandPageSetAPD(ui->CommandPageSetAPDLineEdit->text());
}

void MainWindow::HandleCommandPageSetAPD(const QString& input) {
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int apd = input.toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  if (apd > 200 || apd < 90) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetAPD(apd);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetVdbsPushButton_clicked()
{
  return HandleCommandPageSetVdbs(ui->CommandPageSetVdbsLineEdit->text());
}

void MainWindow::HandleCommandPageSetVdbs(const QString& input) {
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = input.toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetVdbs(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageResetPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->ResetDevice();
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageSaveSettingsPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SaveSettings();
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageRestoreFactoryPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->RestoreFactory();
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageSetFrequencyPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = ui->CommandPageSetFrequencyLineEdit->text().toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetFrequency(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetCorrAPushButton_clicked()
{
  return HandleCommandPageSetCorrA(ui->CommandPageSetCorrALineEdit->text());
}

void MainWindow::HandleCommandPageSetCorrA(const QString& input) {
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  double value = input.toDouble(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetTableCorrA(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetCorrBPushButton_clicked()
{
  return HandleCommandPageSetCorrB(ui->CommandPageSetCorrBLineEdit->text());
}

void MainWindow::HandleCommandPageSetCorrB(const QString& input) {
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = input.toInt(&ok);
  if (!ok) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  sensor_driver_mutex_.lock();
  sensor_driver_->SetTableCorrB(value);
  sensor_driver_mutex_.unlock();
  return;
}

void MainWindow::on_CommandPageSetProtocolPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  tf03_driver::ProtocolType type;
  if (ui->CommandPageSetProtocolComboBox->currentText() ==
      kCommandSetProtocolReleaseProtocol) {
    type = tf03_driver::ProtocolType::release;
  } else if (ui->CommandPageSetProtocolComboBox->currentText() ==
             kCommandSetProtocolDevelopProtocol) {
    type = tf03_driver::ProtocolType::develop;
  } else {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }

  sensor_driver_mutex_.lock();
  sensor_driver_->SetProtocolType(type);
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageSetTransTypePushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  tf03_driver::TransType type;
  if (ui->CommandPageSetTransTypeComboBox->currentText() ==
      kCommandSetTransTypeSerial) {
    type = tf03_driver::TransType::serial;
  } else if (ui->CommandPageSetTransTypeComboBox->currentText() ==
             kCommandSetTransTypeCAN) {
    type = tf03_driver::TransType::can;
  } else {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }

  sensor_driver_mutex_.lock();
  sensor_driver_->SetTransType(type);
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageWriteParamPushButton_clicked()
{
  constexpr int kSplineBreaksNum = 9;
  constexpr int kSplineCoefsRow = 4;
  constexpr int kSplineCoefsColum = 8;

  QString file;
  auto line_edit = ui->CommandPageWriteParamLineEdit;
  auto ui_path = line_edit->text();
  if (ui_path.isEmpty()) {
    file = QtHelpers::ChooseFile(this);
  } else {
    file = ui_path;
  }
  if (file.isEmpty()) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  ui->CommandPageWriteParamLineEdit->setText(file);
  QFile qfile(file);
  if (!qfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }
  QString line;
  QTextStream stream(&qfile);
  std::vector<int16_t> breaks;
  std::vector<std::vector<int32_t>> coefs;
  const QString rawdist_std_identifier = "rawdist_std:";

  int16_t rawdist_std = 0;

  while (!stream.atEnd()) {
    line = stream.readLine();

    if (line.contains(rawdist_std_identifier)) {
      line.remove(rawdist_std_identifier);
      line.remove(" ");
      bool ok;
      rawdist_std = line.toInt(&ok);
      if (!ok) {
        QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
        return;
      }
    }

    if (line == "spline breaks:") {
      breaks.clear();
      line = stream.readLine();
      QStringList list = line.split(" ", QString::SkipEmptyParts);
      if (list.size() != kSplineBreaksNum) {
        QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
        return;
      }
      for (int i = 0; i < list.size(); ++i) {
        bool ok;
        QString str = list[i];
        auto integer = str.toInt(&ok);
        if (!ok) {
          QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
          return;
        }
        breaks.push_back((int16_t)integer);
      }
      continue;
    }

    if (line == "spline coefs:") {
      coefs.clear();
      for (int row = 0; row < kSplineCoefsRow; ++row) {
        std::vector<int32_t> column;
        line = stream.readLine();
        QStringList list = line.split(" ", QString::SkipEmptyParts);
        if (list.size() != kSplineCoefsColum) {
          QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
          return;
        }
        for (int i = 0; i < list.size(); ++i) {
          bool ok;
          QString str = list[i];
          auto int_value = str.toInt(&ok);
          if (!ok) {
            QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
            return;
          }
          column.push_back(int_value);
        }
        coefs.push_back(column);
      }
    }
  }

  breaks.push_back(rawdist_std);

  sensor_driver_mutex_.lock();
  sensor_driver_->SetSplineBreaks(breaks);
  sensor_driver_mutex_.unlock();
  QThread::msleep(100);
  sensor_driver_mutex_.lock();
  sensor_driver_->SetSplineCoefs(coefs);
  sensor_driver_mutex_.unlock();
}

void MainWindow::HandleCommandPageEchoUpdate() {
  static int apd_cnt = 0;
  static int vdbs_cnt = 0;
  static int corr_a_cnt = 0;
  static int corr_b_cnt = 0;
  static int protocol_cnt = 0;
  static int trans_type_cnt = 0;
  static int spline_breaks_cnt = 0;
  static int spline_coefs_cnt = 0;
  static int version_cnt = 0;
  static int restore_cnt = 0;
  static int reset_cnt = 0;
  static int save_cnt = 0;
  static int vdbs_adjust_cnt = 0;
  static int apd_auto_cnt = 0;

  // sensor_driver_mutex_.lock();
  auto apd_echo = sensor_driver_->set_apd_echo;
  auto vdbs_echo = sensor_driver_->set_vdbs_echo;
  auto corr_a_echo = sensor_driver_->set_corr_a_echo;
  auto corr_b_echo = sensor_driver_->set_corr_b_echo;
  auto protocol_echo = sensor_driver_->set_protocol_echo;
  auto trans_type_echo = sensor_driver_->set_trans_type_echo;
  auto spline_breaks_echo = sensor_driver_->set_spline_breaks_echo;
  auto spline_coefs_echo = sensor_driver_->set_spline_coefs_echo;
  auto version_echo = sensor_driver_->check_version_echo;
  auto restore_echo = sensor_driver_->restore_factory_echo;
  auto reset_echo = sensor_driver_->soft_reset_echo;
  auto save_echo = sensor_driver_->save_settings_echo;
  auto vdbs_adjust_echo = sensor_driver_->vdbs_adjust_echo;
  auto apd_auto_echo = sensor_driver_->apd_auto_echo;
  // sensor_driver_mutex_.unlock();

  if (apd_cnt != apd_echo.size()) {
    auto size = apd_echo.size();
    apd_echo.erase(apd_echo.begin(), apd_echo.begin() + apd_cnt);
    for (auto& echo : apd_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set APD Successful");
      } else {
        CommandPageDumpEcho("Set APD Failed");
      }
    }
    apd_cnt = size;
  }

  if (vdbs_cnt != vdbs_echo.size()) {
    auto size = vdbs_echo.size();
    vdbs_echo.erase(vdbs_echo.begin(), vdbs_echo.begin() + vdbs_cnt);
    for (auto& echo : vdbs_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set Vdbs Successful");
      } else {
        CommandPageDumpEcho("Set Vdbs Failed");
      }
    }
    vdbs_cnt = size;
  }

  if (corr_a_cnt != corr_a_echo.size()) {
    auto size = corr_a_echo.size();
    corr_a_echo.erase(corr_a_echo.begin(), corr_a_echo.begin() + corr_a_cnt);
    for (auto& echo : corr_a_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set CorrA Successful");
      } else {
        CommandPageDumpEcho("Set CorrA Failed");
      }
    }
    corr_a_cnt = size;
  }

  if (corr_b_cnt != corr_b_echo.size()) {
    auto size = corr_b_echo.size();
    corr_b_echo.erase(corr_b_echo.begin(), corr_b_echo.begin() + corr_b_cnt);
    for (auto& echo : corr_b_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set CorrB Successful");
      } else {
        CommandPageDumpEcho("Set CorrB Failed");
      }
    }
    corr_b_cnt = size;
  }

  if (protocol_cnt != protocol_echo.size()) {
    auto size = protocol_echo.size();
    protocol_echo.erase(protocol_echo.begin(), protocol_echo.begin() + protocol_cnt);
    for (auto& echo : protocol_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set Protocol Successful");
      } else {
        CommandPageDumpEcho("Set Protocol Failed");
      }
    }
    protocol_cnt = size;
  }

  if (trans_type_cnt != trans_type_echo.size()) {
    auto size = trans_type_echo.size();
    trans_type_echo.erase(trans_type_echo.begin(), trans_type_echo.begin() + trans_type_cnt);
    for (auto& echo : trans_type_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set Trans Type Successful");
      } else {
        CommandPageDumpEcho("Set Trans Type Failed");
      }
    }
    trans_type_cnt = size;
  }

  if (spline_breaks_cnt != spline_breaks_echo.size()) {
    auto size = spline_breaks_echo.size();
    spline_breaks_echo.erase(spline_breaks_echo.begin(), spline_breaks_echo.begin() + spline_breaks_cnt);
    for (auto& echo : spline_breaks_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set Spline Breaks Successful");
      } else {
        CommandPageDumpEcho("Set Spline Breaks Failed");
      }
    }
    spline_breaks_cnt = size;
  }

  if (spline_coefs_cnt != spline_coefs_echo.size()) {
    auto size = spline_coefs_echo.size();
    spline_coefs_echo.erase(spline_coefs_echo.begin(), spline_coefs_echo.begin() + spline_coefs_cnt);
    for (auto& echo : spline_coefs_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Set Spline Coefs Successful");
      } else {
        CommandPageDumpEcho("Set Spline Coefs Failed");
      }
    }
    spline_coefs_cnt = size;
  }

  if (version_cnt != version_echo.size()) {
    auto size = version_echo.size();
    version_echo.erase(version_echo.begin(), version_echo.begin() + version_cnt);
    for (auto& echo : version_echo) {
      CommandPageDumpEcho(QString::fromStdString("Version: " + echo));
    }
    version_cnt = size;
  }

  if (restore_cnt != restore_echo.size()) {
    auto size = restore_echo.size();
    restore_echo.erase(restore_echo.begin(), restore_echo.begin() + restore_cnt);
    for (auto& echo : restore_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Restore Factory Successful");
      } else {
        CommandPageDumpEcho("Restore Factory Failed");
      }
    }
    restore_cnt = size;
  }

  if (reset_cnt != reset_echo.size()) {
    auto size = reset_echo.size();
    reset_echo.erase(reset_echo.begin(), reset_echo.begin() + reset_cnt);
    for (auto& echo : reset_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Soft Reset Successful");
      } else {
        CommandPageDumpEcho("Soft Reset Failed");
      }
    }
    reset_cnt = size;
  }

  if (save_cnt != save_echo.size()) {
    auto size = save_echo.size();
    save_echo.erase(save_echo.begin(), save_echo.begin() + save_cnt);
    for (auto& echo : save_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Save Settings Successful");
      } else {
        CommandPageDumpEcho("Save Settings Failed");
      }
    }
    save_cnt = size;
  }

  if (vdbs_adjust_cnt != vdbs_adjust_echo.size()) {
    auto size = vdbs_adjust_echo.size();
    vdbs_adjust_echo.erase(vdbs_adjust_echo.begin(), vdbs_adjust_echo.begin() + vdbs_adjust_cnt);
    for (auto& echo : vdbs_adjust_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("Vdbs Adjust Successful");
      } else {
        CommandPageDumpEcho("Vdbs Adjust Failed");
      }
    }
    vdbs_adjust_cnt = size;
  }

  if (apd_auto_cnt != apd_auto_echo.size()) {
    auto size = apd_auto_echo.size();
    apd_auto_echo.erase(apd_auto_echo.begin(), apd_auto_echo.begin() + apd_auto_cnt);
    for (auto& echo : apd_auto_echo) {
      if (echo.success == true) {
        CommandPageDumpEcho("APD Auto Mode Switched Successful");
      } else {
        CommandPageDumpEcho("APD Auto Mode Switched Failed");
      }
    }
    apd_auto_cnt = size;
  }
}

void MainWindow::CommandPageDumpEcho(const QString& msg) {
  static int line = 0;
  QTextCursor cursor = ui->CommandPageEchoPlainTextEdit->textCursor();
  cursor.movePosition(QTextCursor::Start);
  cursor.insertText(QString::number(++line) + ") " + msg + "\n");
}

void MainWindow::on_CommandPageCheckVersionPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }

  sensor_driver_mutex_.lock();
  auto version = sensor_driver_->GetVersion();
  sensor_driver_mutex_.unlock();
}


void MainWindow::on_CommandPageVdbsAdjustPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  tf03_driver::VdbsAdjustType type;
  if (ui->CommandPageVdbsAdjustComboBox->currentText() ==
      kCommandVdbsAdjustTypeDisabled) {
    type = tf03_driver::VdbsAdjustType::disabled;
  } else if (ui->CommandPageVdbsAdjustComboBox->currentText() ==
             kCommandVdbsAdjustTypeAlgorithm) {
    type = tf03_driver::VdbsAdjustType::algorithm;
  } else if (ui->CommandPageVdbsAdjustComboBox->currentText() ==
             kCommandVdbsAdjustTypeTDC) {
    type = tf03_driver::VdbsAdjustType::tdc;
  } else {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }

  sensor_driver_mutex_.lock();
  sensor_driver_->AdjustVdbs(type);
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageAPDAutoPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool enable;
  if (ui->CommandPageAPDAutoComboBox->currentText() ==
      kCommandAPDAutoEnable) {
    enable = true;
  } else if (ui->CommandPageAPDAutoComboBox->currentText() ==
             kCommandAPDAutoDisable) {
    enable = false;
  } else {
    QMessageBox::warning(this, "Abort", "Please enter a valid parameter.", QMessageBox::Ok);
    return;
  }

  sensor_driver_mutex_.lock();
  sensor_driver_->EnableAPDAuto(enable);
  sensor_driver_mutex_.unlock();
}

void MainWindow::on_CommandPageAPDExperimentPushButton_clicked()
{
  emit ShowAPDExperimentWindow();
}
