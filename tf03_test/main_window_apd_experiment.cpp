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
    apd_experiment_measurements_list_.clear();
    apd_experiment_measurements_list_.push_back(measurement);
  }
  // apd_experiment_measurements_mutex_.unlock();

  if (IsAPDCrashed(apd_experiment_measurements_list_)) {
    apd_experiment_ended_ = true;
    apd_crashed_ = true;
    apd_crashed_voltage_.store(std::prev(apd_experiment_measurements_list_.end())->apd);
  }
}

bool MainWindow::IsAPDCrashed(const std::list<tf03_driver::Measurement>& list) {
  constexpr int kLeastNumVolt = 10;
  auto iter = list.begin();
  auto& map = apd_voltage_map_;
  if (iter == list.end()) {
    return false;
  }
  std::set<int> new_incomings;
  int volt;
  while (iter != list.end()) {
    volt = iter->apd;
    new_incomings.insert(volt);
    if (!map.count(volt)) {
      map[volt] = std::shared_ptr<std::list<tf03_driver::Measurement>>(new std::list<tf03_driver::Measurement>);
    }
    if (map[volt]->size() < kLeastNumVolt) {
      map[volt]->push_back(*iter);
    }
    ++iter;
  }

  if (map.size() < 2) {
    return false;
  }
  const int offset_from = std::next(map.begin())->first;
  if (apd_stand_dist_ < 0.0f) {
    if (!map.count(offset_from)) {
      return false;
    } else {
      if (map[offset_from]->size() < kLeastNumVolt) {
        return false;
      } else {
        apd_stand_dist_ = RobustAverageDist(map[offset_from]);
//        std::cout << offset_from << " " << apd_stand_dist_ << std::endl;
      }
    }
  }

  for (auto volt : new_incomings) {
    if (map[volt]->size() < kLeastNumVolt) {
      continue;
    }
    auto std_dist = RobustSTD(map[volt], apd_stand_dist_);
//    std::cout << "std_dist " << std_dist << std::endl;
    if (std_dist > 35) {
      return true;
    }
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
