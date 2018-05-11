#ifndef DISTANCE_OVER_TIME_MULTI_SERIES_CHART_H
#define DISTANCE_OVER_TIME_MULTI_SERIES_CHART_H

#include "distance_over_time_chart.h"
#include <unordered_map>

namespace tf0x_common {
class API DistanceOverTimeMultiSeriesChart : public DistanceOverTimeChart
{
public:
  DistanceOverTimeMultiSeriesChart();
  virtual ~DistanceOverTimeMultiSeriesChart();
  bool AddPoint(
      const float &meter, const int &millisecond, const std::string& id);
  bool RemoveSeries(const std::string& id);
private:
  std::unordered_map<std::string, QtCharts::QLineSeries*> series_map_;
};
} // namespace tf0x_common

#endif // DISTANCE_OVER_TIME_MULTI_SERIES_CHART_H
