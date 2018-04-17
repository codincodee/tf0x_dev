#ifndef DISTANCE_OVER_TIME_AND_SWITCH_VALUE_CHART_H
#define DISTANCE_OVER_TIME_AND_SWITCH_VALUE_CHART_H

#include "distance_over_time_chart.h"
#include <QValueAxis>

namespace tf0x_common {
class API DistanceOverTimeAndSwitchValueChart : public DistanceOverTimeChart {
public:
  DistanceOverTimeAndSwitchValueChart();
  virtual ~DistanceOverTimeAndSwitchValueChart();
  bool AddSwitchValuePoint(const bool& on, const int& msec);
private:
  QtCharts::QLineSeries* line_series_;
  QtCharts::QValueAxis* axis_y_;
};
} // tf0x_common

#endif // DISTANCE_OVER_TIME_AND_SWITCH_VALUE_CHART_H
