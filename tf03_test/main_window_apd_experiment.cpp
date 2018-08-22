#include "main_window.h"
#include "ui_main_window.h"
#include <set>
#include <iterator>
#include <algorithm>
#include <iostream>

void MainWindow::HandleAPDExperiment(const std::vector<tf03_driver::Measurement> &measurements) {
  if (!apd_experiment_on_) {
    return;
  }
  if (apd_experiment_ended_) {
    return;
  }
  // apd_experiment_measurements_mutex_.lock();
  for (auto& measurement : measurements) {
    apd_experiment_measurements_list_.push_back({measurement, apd_anticipated_voltage_});
  }
  // apd_experiment_measurements_mutex_.unlock();

  if (apd_experiment_last_anticipated_voltage_ == apd_anticipated_voltage_) {
    return;
  }
  apd_experiment_last_anticipated_voltage_ = apd_anticipated_voltage_;

  if (IsAPDCrashed(apd_experiment_measurements_list_)) {
    apd_experiment_ended_ = true;
    apd_crashed_ = true;
    // apd_crashed_voltage_.store(std::prev(apd_experiment_measurements_list_.end())->apd);
    apd_crashed_voltage_.store(apd_anticipated_voltage_ - 1);
    if (!apd_experiment_measurements_list_.empty()) {
      apd_crashed_temperature_.store(apd_experiment_measurements_list_.rbegin()->first.temp);
      apd_experiment_result_voltage_.store(ComputeAPDExperimentResultVoltage(apd_crashed_voltage_.load(), apd_crashed_temperature_.load()));
    } else {
      apd_experiment_result_voltage_.store(0);
    }
  }
}

bool MainWindow::IsAPDCrashed(const std::list<std::pair<tf03_driver::Measurement, float>>& list) {
  if (list.empty()) {
    return false;
  }
  if (apd_stand_dist_ < 0.0f) {
    if (apd_voltage_from_ >= list.rbegin()->second) {
      auto iter = list.begin();
      std::shared_ptr<std::list<tf03_driver::Measurement>> samples(new std::list<tf03_driver::Measurement>);
      while (iter->second < list.rbegin()->second) {
        if (iter == list.end()) {
          break;
        }
        samples->push_back(iter->first);
        ++iter;
      }
      apd_stand_dist_ = RobustAverageDist(samples);
      if (apd_stand_dist_ < 0.0) {
        return false;
      }
    } else {
      return false;
    }
  }

  std::cout << "apd stand dist: " << apd_stand_dist_ << std::endl;

  std::shared_ptr<std::list<tf03_driver::Measurement>> samples(new std::list<tf03_driver::Measurement>);
  float voltage_of_interest = 0.0f;
  for (auto& iter = list.rbegin(); iter != list.rend(); ++iter) {
    if (iter->second != list.rbegin()->second) {
      voltage_of_interest = iter->second;
      break;
    }
  }
  for (auto& iter = list.rbegin(); iter != list.rend(); ++iter) {
    if (iter->second == voltage_of_interest) {
      samples->push_back(iter->first);
    }
    if (iter->second < voltage_of_interest) {
      break;
    }
  }

  auto result = RobustSTD(samples, apd_stand_dist_);
  std::cout << "result: " << result << std::endl;
  if (result >= apd_experiment_crash_threshold_) {
    return true;
  }
  return false;
}

int MainWindow::RobustAverageDist(
    std::shared_ptr<std::list<tf03_driver::Measurement> > dists) {
  if (!dists) {
    return -1;
  }
  if (dists->size() <= kMaxToIgnore + kMinToIngore) {
    return -1;
  }
  dists->sort([](const tf03_driver::Measurement& a, const tf03_driver::Measurement& b){return a.raw_dist1 > b.raw_dist1;});
  int total = 0;
  // auto begin = dists->begin();
  auto begin = std::next(dists->begin(), kMaxToIgnore);
  auto end = std::prev(dists->end(), kMinToIngore);
  for (auto i = begin; i != end; ++i) {
    total += i->raw_dist1;
  }
  return total * 1.0f / (dists->size() - kMaxToIgnore - kMinToIngore);
}

int MainWindow::RobustSTD(std::shared_ptr<std::list<tf03_driver::Measurement> > dists, const int &std_dist) {
  if (!dists) {
    return -1;
  }
  if (dists->size() <= kMaxToIgnore + kMinToIngore) {
    return -1;
  }
  std::vector<float> dist_vec;
  dist_vec.reserve(dists->size());
  for (auto& i : *dists) {
    dist_vec.push_back(i.raw_dist1 * 1.0f / std_dist);
  }
  std::sort(dist_vec.begin(), dist_vec.end());
  auto begin = std::next(dist_vec.begin(), kMaxToIgnore);
  auto end = std::prev(dist_vec.end(), kMinToIngore);

  int num = dist_vec.size() - kMaxToIgnore - kMinToIngore;
  float mean = 0;
  for (auto i = begin; i != end; ++i) {
    mean += *i;
  }
  mean = mean / num;

  float stddev = 0;
  float diff;
  for (auto i = begin; i != end; ++i) {
    diff = *i - mean;
    stddev += diff * diff;
  }
  return std::sqrt(stddev / num) * 120;
}

float MainWindow::ComputeAPDExperimentResultVoltage(const float &volt, const float &temp) {
  return (volt - (temp - 30) * 0.9) * 0.9;
}
