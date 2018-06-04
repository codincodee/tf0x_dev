#include "main_window.h"
#include "ui_main_window.h"
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QDesktopServices>

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

const QString kCartStartButtonStart = "Start";
const QString kCartStartButtonReset = "Reset";

void MainWindow::HandleCartInstruction(
    const cart_driver::Instruction &instruction, const int &repetition) {
  if (instruction.type == cart_driver::Instruction::Type::read_sensor) {
    auto sheet = cart_results_->CurrentSheet();
    if (sheet) {
      tf03_driver::CartMeasurement cart_measure;
      if (!cart_log_.empty()) {
        cart_measure.id = (cart_log_.end() - 1)->id;
        cart_measure.pos = (cart_log_.end() - 1)->pos;
      }
      cart_measure.measurement = last_measurement_;
      for (int i = 0; i < repetition; ++i) {
        ++cart_measure.id;
        cart_measure.pos += cart_driver_->StopInterval() * 100;

        tf0x_common::CartTestEntry entry;
        entry.id = cart_measure.id;
        entry.pos = cart_measure.pos;
        entry.dist = cart_measure.measurement.dist1;
        sheet->AddEntry(entry);
        cart_log_.push_back(cart_measure);
//        if (sheet->Size() > 30) {
//          sheet->Clear();
//          int i = 0;
//          for (auto& it = cart_log_.rbegin(); it != cart_log_.rend(); ++it) {
//            tf0x_common::CartTestEntry entry;
//            entry.id = it->id;
//            entry.pos = it->pos;
//            entry.dist = it->measurement.dist1;
//            // sheet->AddEntry(entry);
//            ++i;
//            if (i > 30) {
//              break;
//            }
//          }
//        }
      }
    }
  } else if (instruction.type ==
             cart_driver::Instruction::Type::reach_end_point) {
    SaveCartTestLog("_forward");
    cart_results_->SheetDone();
    // ui->CartInfoLabel->setText("Returning");
    ui->CartStartTestPushButton->setText(kCartStartButtonStart);
    ui->CartPageLogFileLineEdit->clear();
  } else if (instruction.type ==
             cart_driver::Instruction::Type::reach_start_point) {
//    if (!cart_test_started_) {
//      cart_test_started_ = true;
//      return;
//    }
//    SaveCartTestLog("_backward");
//    ui->CartPageLogFileLineEdit->clear();
//    cart_results_->SheetDone();
//    ui->CartStartTestPushButton->setText(kCartStartButtonStart);
//    ui->CartInfoLabel->setText("");
//    cart_test_started_ = false;
  }
}

//void MainWindow::HandleCartInstruction(
//    const cart_driver::Instruction &instruction) {
//  if (instruction.type == cart_driver::Instruction::Type::read_sensor) {
//    auto sheet = cart_results_->CurrentSheet();
//    if (sheet) {
//      auto last = sheet->GetLastEntry();
//      if (!last_measurement_.dists.empty()) {
//        tf0x_common::CartTestEntry entry;
//        entry.id = ++last.id;
//        entry.dist = last_measurement_.dists[0];
//        entry.pos = last.pos + cart_driver_->StopInterval();
//        sheet->AddEntry(entry);
//        ui->CartInfoLabel->setText(
//            "Collected " + QString::number(sheet->Size()) + " Points.");
//      }
//    }
//  } else if (instruction.type ==
//             cart_driver::Instruction::Type::reach_end_point) {
//    SaveCartTestLog("_forward");
//    cart_results_->SheetDone();
//    ui->CartInfoLabel->setText("Returning");
//  } else if (instruction.type ==
//             cart_driver::Instruction::Type::reach_start_point) {
//    SaveCartTestLog("_backward");
//    ui->CartPageLogFileLineEdit->clear();
//    cart_results_->SheetDone();
//    ui->CartStartTestPushButton->setText(kCartStartButtonStart);
//    ui->CartInfoLabel->setText("");
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
//      auto sheet = cart_results_->CurrentSheet();
//      if (sheet) {
//        sheet->Clear();
//      }
      // cart_test_started_ = false;
      ui->CartStartTestPushButton->setText(kCartStartButtonStart);
      return;
    }
  }
  cart_results_->Clear();
  cart_log_.clear();
  if (!cart_driver_) {
    return;
  }
  bool distance_ok, interval_ok;
  auto distance = ui->SettingsCartDistanceLineEdit->text().toFloat(&distance_ok);
  auto interval = ui->SettingsCartIntervalLineEdit->text().toFloat(&interval_ok);
  if (!distance_ok || !interval_ok) {
    QMessageBox::warning(
        this, "Error",
         "Invalid parameters set, cart disabled", QMessageBox::Abort);
    return;
  }
  cart_driver_->StartMultiStopsTesting(distance, interval);
  ui->CartStartTestPushButton->setText(kCartStartButtonReset);
}

void MainWindow::SaveCartTestLog(const QString& suffix) {
  QString file_name = ui->CartPageLogFileLineEdit->text();
  if (file_name.isEmpty()) {
    QMessageBox::warning(
        this, "Warning",
        "Please enter a valid log file name.", QMessageBox::Abort);
    return;
  }
  QFile file(
      ui->LogPathLineEdit->text() + "/cart_test_" + file_name + suffix + ".txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    auto sheet = cart_results_->CurrentSheet();
    if (!sheet) {
      return;
    }
//    for (int i = 0; i < sheet->Size(); ++i) {
//      auto entry = sheet->At(i);
//      stream << entry.dist << " " << entry.pos << "\n";
//    }
  } else {
    QMessageBox::warning(
        this, "Error", "Fail to write log.", QMessageBox::Abort);
  }
//  ui->CartPageLogFileLineEdit->clear();
}

void MainWindow::on_CartPageBrowsePushButton_clicked()
{
  QDesktopServices::openUrl(ui->LogPathLineEdit->text());
}

void MainWindow::on_CartPageDistanceLineEdit_textEdited(const QString &arg1)
{
  ui->SettingsCartDistanceLineEdit->setText(arg1);
}
