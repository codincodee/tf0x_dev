#ifndef DISTANCE_OVER_TIME_CHART_H
#define DISTANCE_OVER_TIME_CHART_H

#include "export.h"
#include <QtCharts/qchart.h>

namespace tf0x_common {
class API DistanceOverTimeChart : public QtCharts::QChart {
  Q_OBJECT
public:
  DistanceOverTimeChart();
  void AddPoint(const float& meter, const int& millisecond);
  virtual ~DistanceOverTimeChart();
  void SetTimeWindow(const int& millisecond);
};
} // namespace tf0x_common

#endif // DISTANCE_OVER_TIME_CHART_H
