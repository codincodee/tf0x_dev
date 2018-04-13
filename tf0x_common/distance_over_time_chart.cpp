#include "distance_over_time_chart.h"
#include <QValueAxis>

namespace tf0x_common {
DistanceOverTimeChart::~DistanceOverTimeChart() {
  if (line_series_) {
    delete line_series_;
    line_series_ = nullptr;
  }
}

bool DistanceOverTimeChart::AddPoint(
    const float &meter, const int &millisecond) {
  if (!line_series_) {
    return false;
  }
  if (line_series_->count() > 0) {
    if (millisecond <= line_series_->at(line_series_->count() - 1).x()) {
      return false;
    }
  }
  *line_series_ << QPointF(millisecond, meter);

  while (line_series_->count()) {
    if ((line_series_->at(0).x() + 500) < millisecond) {
      line_series_->removePoints(0, 1);
    } else {
      break;
    }
  }

  if (!line_series_->count()) {
    return false;
  }

  float min = line_series_->at(0).y();
  float max = min;
  for (auto i = 0; i < line_series_->count(); ++i) {
    if (line_series_->at(i).y() > max) {
      max = line_series_->at(i).y();
    }
    if (line_series_->at(i).y() < min) {
      min = line_series_->at(i).y();
    }
  }
  if (max > 20.0f) {
    max = 20.0f;
  }
  if (max < 5.0f) {
    max = 5.0f;
  }

  // To make the plot look nicer, we currently set MIN to 0.0f;
  min = 0.0f;

  this->removeSeries(line_series_);
  this->addSeries(line_series_);

  auto axis_y = this->axisY();
  axis_y->setRange(min, max);
  this->setAxisY(axis_y, line_series_);
}

DistanceOverTimeChart::DistanceOverTimeChart() {
  line_series_ = new QtCharts::QLineSeries;
  this->addSeries(line_series_);
  this->setTitle("Distance (m) - Time Elapse (ms)");
  this->legend()->hide();
  auto axis_y = new QtCharts::QValueAxis;
  axis_y->setGridLineVisible(true);
  axis_y->setTickCount(20);
  this->addAxis(axis_y, Qt::AlignLeft);
}
} // namespace tf0x_common
