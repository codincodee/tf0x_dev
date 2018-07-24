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

  if (IsAPDCrashed(apd_experiment_measurements_list_)) {
    apd_experiment_ended_ = true;
    apd_crashed_ = true;
    apd_crashed_voltage_.store(apd_anticipated_voltage_);
  }
}

bool MainWindow::IsAPDCrashed(const std::list<tf03_driver::Measurement>& list) {
//  constexpr int kLeastNumVolt = 10;
//  auto& iter = apd_experiment_measurements_list_iterator_;
//  auto& map = apd_voltage_map_;
//  if (iter == list.end()) {
//    return false;
//  }
//  while (iter != list.end()) {
//    auto volt = iter->volt;
//    if (!map.count(volt)) {
//      map[volt] = std::shared_ptr<std::list<tf03_driver::Measurement>>(new std::list<tf03_driver::Measurement>);
//    }
//    if (map[volt]->size() < kLeastNumVolt) {
//      map[volt]->push_back(*iter);
//    }
//  }

//  if (apd_stand_dist_ < 0.0f) {
//    if (!map.count(apd_voltage_from_)) {
//      return false;
//    } else {
//      if (map[apd_voltage_from_]->size() < kLeastNumVolt) {
//        return false;
//      } else {
//      }
//    }
//  }

  return false;
}
