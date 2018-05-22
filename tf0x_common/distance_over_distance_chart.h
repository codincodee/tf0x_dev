#ifndef TF0X_COMMON_DISTANCE_OVER_DISTANCE_CHART_H_
#define TF0X_COMMON_DISTANCE_OVER_DISTANCE_CHART_H_

#include <QtCharts/QChart>
#include "export.h"
#include <QtCharts/QLineSeries>

namespace tf0x_common {
class API FixedDistanceOverDistanceChart : public QtCharts::QChart
{
  Q_OBJECT
 public:
  FixedDistanceOverDistanceChart();
  virtual ~FixedDistanceOverDistanceChart();
  void SetXRange(const float& from, const float& to);
  void SetYRange(const float& from, const float& to);
  void AddPoint(const float& x, const float& y);
 private:
  float x_min_;
  float x_max_;
  float y_min_;
  float y_max_;

  QtCharts::QLineSeries* line_series_;
};
} // namespace tf0x_common

#endif // TF0X_COMMON_DISTANCE_OVER_DISTANCE_CHART_H_
