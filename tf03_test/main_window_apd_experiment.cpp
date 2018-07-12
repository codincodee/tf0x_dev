#include "main_window.h"
#include "ui_main_window.h"

void MainWindow::HandleAPDExperiment(const std::vector<tf03_driver::Measurement> &measurements) {
  if (!apd_experiment_on_) {
    return;
  }
  if (apd_experiment_ended_) {
    return;
  }
  // apd_experiment_measurements_mutex_.lock();
  for (auto& measurement : measurements) {
    apd_experiment_measurements_list_.push_back(measurement);
  }
  // apd_experiment_measurements_mutex_.unlock();

  if (IsAPDCrashed()) {
    apd_experiment_ended_ = true;
    apd_crashed_ = true;
    apd_crashed_voltage_.store(apd_anticipated_voltage_);
  }
}

bool MainWindow::IsAPDCrashed() {
  return false;
}
