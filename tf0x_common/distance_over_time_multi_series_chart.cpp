#include "distance_over_time_multi_series_chart.h"

namespace tf0x_common {
DistanceOverTimeMultiSeriesChart::DistanceOverTimeMultiSeriesChart() {}

DistanceOverTimeMultiSeriesChart::~DistanceOverTimeMultiSeriesChart() {
  for (auto& series : series_map_) {
    if (series.second) {
      delete series.second;
    }
  }
  series_map_.clear();
}

bool DistanceOverTimeMultiSeriesChart::AddPoint(
    const float &meter, const int &millisecond, const std::string &id) {
  if (!series_map_.count(id)) {
    series_map_[id] = new QtCharts::QLineSeries;
  }
  return DistanceOverTimeChart::AddPoint(meter, millisecond, series_map_[id]);
}

bool DistanceOverTimeMultiSeriesChart::RemoveSeries(const std::string &id) {
  if (series_map_.count(id)) {
    this->removeSeries(series_map_[id]);
    series_map_.erase(id);
  }
  return true;
}
} // namespace tf0x_common
