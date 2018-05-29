#include "main_window.h"
#include "ui_main_window.h"
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QDesktopServices>

void MainWindow::InitializeCartPage() {
  cart_chart_ = new tf0x_common::FixedDistanceOverDistanceChart();
  cart_chart_->setTitle("Sensor Dist (m) - Cart Dist (m)");
  cart_chart_view_ = new QtCharts::QChartView(cart_chart_);
  // ui->CartChartVerticalLayout->addWidget(cart_chart_view_);

  cart_results_.reset(new tf0x_common::CartTestMultiResultSheets);
  cart_results_->AddSheet(
      std::shared_ptr<tf0x_common::CartTestResultSheet>(
          new tf0x_common::CartTestResultSheet(
              ui->CartResult1IDListWidget,
              ui->CartResult1PositionListWidget,
              ui->CartResult1DistanceListWidget)));
  cart_results_->AddSheet(
      std::shared_ptr<tf0x_common::CartTestResultSheet>(
          new tf0x_common::CartTestResultSheet(
              ui->CartResult2IDListWidget,
              ui->CartResult2PositionListWidget,
              ui->CartResult2DistanceListWidget)));
}

const QString kCartStartButtonStart = "Start";
const QString kCartStartButtonReset = "Reset";

void MainWindow::HandleCartInstruction(
    const cart_driver::Instruction &instruction) {
  if (instruction.type == cart_driver::Instruction::Type::read_sensor) {
    auto sheet = cart_results_->CurrentSheet();
    if (sheet) {
      auto last = sheet->GetLastEntry();
      if (!last_measurement_.dists.empty()) {
//        auto current_pose = last.x() + cart_driver_->StopInterval();
//        cart_chart_->AddPoint(current_pose, last_measurement_.dists[0]);
//        ui->CartInfoLabel->setText(
//            "Collected " + QString::number(series->count()) + " Points.");
        tf0x_common::CartTestEntry entry;
        entry.id = ++last.id;
        entry.dist = last_measurement_.dists[0];
        entry.pos = last.pos + cart_driver_->StopInterval();
        sheet->AddEntry(entry);
        ui->CartInfoLabel->setText(
            "Collected " + QString::number(sheet->Size()) + " Points.");
      }
    }
  } else if (instruction.type ==
             cart_driver::Instruction::Type::reach_end_point) {
    SaveCartTestLog();
    cart_results_->SheetDone();
    ui->CartStartTestPushButton->setText(kCartStartButtonStart);
    ui->CartInfoLabel->setText("Returning");
  } else if (instruction.type ==
             cart_driver::Instruction::Type::reach_start_point) {
    ui->CartInfoLabel->setText("");
  }
}

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
    }
  }
  cart_chart_->Clear();
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
  cart_chart_->SetXRange(0, distance * 1.1f);
  cart_chart_->SetYRange(0, distance * 1.1f);
  cart_driver_->StartMultiStopsTesting(distance, interval);
  ui->CartStartTestPushButton->setText(kCartStartButtonReset);
}

void MainWindow::SaveCartTestLog() {
  QString file_name = ui->CartPageLogFileLineEdit->text();
  if (file_name.isEmpty()) {
    QMessageBox::warning(
        this, "Warning",
        "Please enter a valid log file name.", QMessageBox::Abort);
    return;
  }
  QFile file(
      ui->LogPathLineEdit->text() + "/" + file_name + ".txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    auto series = cart_chart_->Series();
    if (!series) {
      return;
    }
    for (int i = 0; i < series->count(); ++i) {
      auto point = series->at(i);
      stream << point.x() << " " << point.y() << "\n";
    }
  } else {
    QMessageBox::warning(
        this, "Error", "Fail to write log.", QMessageBox::Abort);
  }
}

void MainWindow::on_CartPageBrowsePushButton_clicked()
{
  QDesktopServices::openUrl(ui->LogPathLineEdit->text());
}

void MainWindow::on_CartPageDistanceLineEdit_textEdited(const QString &arg1)
{
  ui->SettingsCartDistanceLineEdit->setText(arg1);
}
