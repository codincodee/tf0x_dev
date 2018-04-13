#ifndef DISTANCE_OVER_TIME_CHART_H
#define DISTANCE_OVER_TIME_CHART_H

#include "export.h"
#include <QtCharts/qchart.h>
#include <QLineSeries>

namespace tf0x_common {
class API DistanceOverTimeChart : public QtCharts::QChart {
  Q_OBJECT
public:
  DistanceOverTimeChart();
  virtual ~DistanceOverTimeChart();
  bool AddPoint(const float& meter, const int& millisecond);
  void SetTimeWindow(const int& millisecond);
private:
  QtCharts::QLineSeries* line_series_;
};
} // namespace tf0x_common

#endif // DISTANCE_OVER_TIME_CHART_H
