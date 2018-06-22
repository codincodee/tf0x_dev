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

const QString kCommandSetProtocolReleaseProtocol = "Release";
const QString kCommandSetProtocolDevelopProtocol = "Develop";

const QString kCommandSetTransTypeSerial = "Serial";
const QString kCommandSetTransTypeCAN = "CAN";

void MainWindow::InitializeCommandPageCartSection() {
  ui->CommandPageSetProtocolComboBox->addItem(kCommandSetProtocolReleaseProtocol);
  ui->CommandPageSetProtocolComboBox->addItem(kCommandSetProtocolDevelopProtocol);

  ui->CommandPageSetTransTypeComboBox->addItem(kCommandSetTransTypeCAN);
  ui->CommandPageSetTransTypeComboBox->addItem(kCommandSetTransTypeSerial);

  // ui->CommandPageWriteParamLineEdit->setReadOnly(true);
}

void MainWindow::on_CommandPageSetAPDPushButton_clicked()
{
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int apd = ui->CommandPageSetAPDLineEdit->text().toInt(&ok);
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
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = ui->CommandPageSetVdbsLineEdit->text().toInt(&ok);
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
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  double value = ui->CommandPageSetCorrALineEdit->text().toDouble(&ok);
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
  if (!sensor_driver_) {
    return;
  }
  bool ok;
  int value = ui->CommandPageSetCorrBLineEdit->text().toInt(&ok);
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
