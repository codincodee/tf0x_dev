#include "distance_over_distance_chart.h"
#include <QtCharts/QValueAxis>
#include <QDebug>

namespace tf0x_common {
FixedDistanceOverDistanceChart::FixedDistanceOverDistanceChart()
  : line_series_(nullptr), x_min_(0), x_max_(10), y_min_(0), y_max_(10) {
  line_series_ = new QtCharts::QLineSeries;
  this->addSeries(line_series_);
  this->setTitle("Distance (m) - Distance (m)");
  this->legend()->hide();
  auto axis_y = new QtCharts::QValueAxis;
  axis_y->setGridLineVisible(true);
  axis_y->setTickCount(20);
  this->addAxis(axis_y, Qt::AlignLeft);
  auto axis_x = new QtCharts::QValueAxis;
  axis_x->setGridLineVisible(true);
  axis_x->setTickCount(20);
  this->addAxis(axis_x, Qt::AlignBottom);
}

FixedDistanceOverDistanceChart::~FixedDistanceOverDistanceChart() {
  if (line_series_) {
    delete line_series_;
    line_series_ = nullptr;
  }
}

void FixedDistanceOverDistanceChart::SetXRange(
    const float &from, const float &to) {
  x_min_ = from;
  x_max_ = to;
  if (!line_series_) {
    return;
  }
  auto axis = this->axisX();
  axis->setRange(x_min_, x_max_);
  this->setAxisX(axis, line_series_);
}

void FixedDistanceOverDistanceChart::SetYRange(
    const float &from, const float &to) {
  y_min_ = from;
  y_max_ = to;
  if (!line_series_) {
    return;
  }
  auto axis = this->axisY();
  axis->setRange(y_min_, y_max_);
  this->setAxisY(axis, line_series_);
}

void FixedDistanceOverDistanceChart::AddPoint(const float &x, const float &y) {
  if (!line_series_) {
    return;
  }
  *line_series_ << QPointF(x, y);
}

QtCharts::QLineSeries* FixedDistanceOverDistanceChart::Series() {
  return line_series_;
}
} // namespace tf0x_common
