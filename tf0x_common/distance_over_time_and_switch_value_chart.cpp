#include "distance_over_time_and_switch_value_chart.h"

namespace tf0x_common {
DistanceOverTimeAndSwitchValueChart::DistanceOverTimeAndSwitchValueChart() {
  line_series_ = new QtCharts::QLineSeries;
  this->addSeries(line_series_);
}
DistanceOverTimeAndSwitchValueChart::~DistanceOverTimeAndSwitchValueChart() {
  if (line_series_) {
    delete line_series_;
    line_series_ = nullptr;
  }
}

bool DistanceOverTimeAndSwitchValueChart::AddSwitchValuePoint(
    const bool &on, const int &msec) {
  if (on) {
    return
        AddPoint(GetMin() + (GetMax() - GetMin()) / 2.0f, msec, line_series_);
  } else {
    return AddPoint(GetMin(), msec, line_series_);
  }
}
} // namespace tf0x_common
