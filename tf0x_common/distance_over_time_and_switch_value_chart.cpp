#include "distance_over_time_and_switch_value_chart.h"

namespace tf0x_common {
DistanceOverTimeAndSwitchValueChart::DistanceOverTimeAndSwitchValueChart() {
  line_series_ = new QtCharts::QLineSeries;
  this->addSeries(line_series_);
  axis_y_ = new QtCharts::QValueAxis;
  axis_y_->setRange(0.0f, 3.0f);
  axis_y_->hide();
  this->setAxisY(axis_y_, line_series_);
}

DistanceOverTimeAndSwitchValueChart::~DistanceOverTimeAndSwitchValueChart() {
  if (line_series_) {
    delete line_series_;
    line_series_ = nullptr;
  }
  if (axis_y_) {
    delete axis_y_;
    axis_y_ = nullptr;
  }
}

bool DistanceOverTimeAndSwitchValueChart::AddSwitchValuePoint(
    const bool &on, const int &msec) {
  if (on) {
    // Arbitrary value for the first param
    return AddPoint(1.0f, msec, line_series_);
  } else {
    return AddPoint(GetMin(), msec, line_series_);
  }
}
} // namespace tf0x_common
