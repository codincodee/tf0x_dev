#include "main_window.h"
#include "ui_main_window.h"
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QDesktopServices>

const QString kCartStartButtonStart = "Start";
const QString kCartStartButtonReset = "Reset";

void MainWindow::InitializeCartPage() {
  cart_results_.reset(new tf0x_common::CartTestMultiResultSheets);
  cart_results_->AddSheet(
      std::shared_ptr<tf0x_common::CartTestResultSheet>(
          new tf0x_common::CartTestResultSheet(
              ui->CartResult1ID,
              ui->CartResult1Position,
              ui->CartResult1Distance)));
  cart_results_->AddSheet(
      std::shared_ptr<tf0x_common::CartTestResultSheet>(
          new tf0x_common::CartTestResultSheet(
              ui->CartResult2ID,
              ui->CartResult2Position,
              ui->CartResult2Distance)));
}

void MainWindow::HandleCartTimerEvent() {
  cart_readings_mutex_.lock();
  auto readings = cart_readings_;
  cart_readings_mutex_.unlock();

  if (cart_logging_) {
    auto sheet = cart_results_->CurrentSheet();
    if (sheet) {
      sheet->Clear();
      int i = 0;
      for (auto& it = readings.rbegin(); it != readings.rend(); ++it) {
        ++i;
        if (i > 30) {
          break;
        }
        tf0x_common::CartTestEntry entry;
        entry.id = it->id;
        entry.pos = it->pos;
        entry.dist = it->measurement.dist1;
        sheet->AddEntry(entry);
      }
    }
  }

  if (!cart_logging_ && !readings.empty()) {
    SaveCartTestLog(readings);
    cart_results_->SheetDone();
    ui->CartStartTestPushButton->setText(kCartStartButtonStart);
    cart_readings_mutex_.lock();
    cart_readings_.clear();
    cart_readings_mutex_.unlock();
  }
}

void MainWindow::SaveCartTestLog(
    const std::vector<tf03_driver::CartMeasurement> &readings) {
  QString file_name = ui->CartPageLogFileLineEdit->text();
  if (file_name.isEmpty()) {
    QMessageBox::warning(
        this, "Warning",
        "Please enter a valid log file name.", QMessageBox::Abort);
    return;
  }
  QFile file(
      ui->LogPathLineEdit->text() + "/cart_test_" + file_name + ".txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << "# Position(cm) Distance-1(cm) Distance-2(cm) Distance-3(cm) APD-Voltage(V) Laser-Voltage(V) Temperature(C)\n";
    for (auto& entry : readings) {
      stream
          << entry.pos << " "
          << entry.measurement.dist1 << " "
          << entry.measurement.dist2 << " "
          << entry.measurement.dist3 << " "
          << entry.measurement.apd << " "
          << entry.measurement.volt << " "
          << entry.measurement.temp << "\n";
    }
  } else {
    QMessageBox::warning(
        this, "Error", "Fail to write log.", QMessageBox::Abort);
  }
}

//void MainWindow::HandleCartInstruction(
//    const cart_driver::Instruction &instruction, const int &repetition) {
//  if (instruction.type == cart_driver::Instruction::Type::read_sensor) {
//    auto sheet = cart_results_->CurrentSheet();
//    if (sheet) {
//      tf03_driver::CartMeasurement cart_measure;
//      if (!cart_log_.empty()) {
//        cart_measure.id = (cart_log_.end() - 1)->id;
//        cart_measure.pos = (cart_log_.end() - 1)->pos;
//      }
//      cart_measure.measurement = last_measurement_;
//      for (int i = 0; i < repetition; ++i) {
//        ++cart_measure.id;
//        cart_measure.pos += cart_driver_->StopInterval() * 100;

//        tf0x_common::CartTestEntry entry;
//        entry.id = cart_measure.id;
//        entry.pos = cart_measure.pos;
//        entry.dist = cart_measure.measurement.dist1;
//        sheet->AddEntry(entry);
//        cart_log_.push_back(cart_measure);
//      }
//    }
//  } else if (instruction.type ==
//             cart_driver::Instruction::Type::reach_end_point) {
//    SaveCartTestLog("_forward");
//    cart_results_->SheetDone();
//    ui->CartStartTestPushButton->setText(kCartStartButtonStart);
//    ui->CartPageLogFileLineEdit->clear();
//  } else if (instruction.type ==
//             cart_driver::Instruction::Type::reach_start_point) {
//  }
//}

void MainWindow::on_CartStartTestPushButton_clicked()
{
  if (ui->CartPageLogFileLineEdit->text().isEmpty()) {
    QMessageBox::warning(
        this, "Warning",
        "Please enter a valid log file name.", QMessageBox::Abort);
    return;
  }
  if (ui->CartStartTestPushButton->text() == kCartStartButtonReset) {
    if (QMessageBox::information(
          this, "Alert",
          "Make sure the cart has been reset first.",
          QMessageBox::Cancel, QMessageBox::Apply) == QMessageBox::Cancel) {
      return;
    } else {
      ui->CartStartTestPushButton->setText(kCartStartButtonStart);
      return;
    }
  }

  cart_last_measurement_mutex_.lock();
  cart_last_measurement_ = tf03_driver::CartMeasurement();
  cart_last_measurement_mutex_.unlock();

  cart_logging_ = false;

  cart_readings_mutex_.lock();
  cart_readings_.clear();
  cart_readings_mutex_.unlock();

  if (!cart_driver_) {
    return;
  }
  bool distance_ok, interval_ok;
  auto distance = ui->SettingsCartDistanceLineEdit->text().toInt(&distance_ok) / 100.0f;
  auto interval = ui->SettingsCartIntervalLineEdit->text().toInt(&interval_ok) / 100.0f;
  if (!distance_ok || !interval_ok) {
    QMessageBox::warning(
        this, "Error",
         "Invalid parameters set, cart disabled", QMessageBox::Abort);
    return;
  }
  cart_readings_mutex_.lock();
  cart_readings_.reserve(distance / interval * 1.1);
  cart_readings_mutex_.unlock();

  cart_driver_mutex_.lock();
  cart_driver_->StartMultiStopsTesting(distance, interval);
  cart_driver_mutex_.unlock();
  ui->CartStartTestPushButton->setText(kCartStartButtonReset);
}

//void MainWindow::SaveCartTestLog(const QString& suffix) {
//  QString file_name = ui->CartPageLogFileLineEdit->text();
//  if (file_name.isEmpty()) {
//    QMessageBox::warning(
//        this, "Warning",
//        "Please enter a valid log file name.", QMessageBox::Abort);
//    return;
//  }
//  QFile file(
//      ui->LogPathLineEdit->text() + "/cart_test_" + file_name + suffix + ".txt");
//  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//    QTextStream stream(&file);
//    stream << "# Position(cm) Distance-1(cm) Distance-2(cm) Distance-3(cm) APD-Voltage(V) Laser-Voltage(V) Temperature(C)\n";
//    for (auto& entry : cart_log_) {
//      stream
//          << entry.pos << " "
//          << entry.measurement.dist1 << " "
//          << entry.measurement.dist2 << " "
//          << entry.measurement.dist3 << " "
//          << entry.measurement.apd << " "
//          << entry.measurement.volt << " "
//          << entry.measurement.temp << "\n";
//    }
//  } else {
//    QMessageBox::warning(
//        this, "Error", "Fail to write log.", QMessageBox::Abort);
//  }
//}

void MainWindow::on_CartPageBrowsePushButton_clicked()
{
  QDesktopServices::openUrl(ui->LogPathLineEdit->text());
}

void MainWindow::on_CartPageDistanceLineEdit_textEdited(const QString &arg1)
{
  ui->SettingsCartDistanceLineEdit->setText(arg1);
}
