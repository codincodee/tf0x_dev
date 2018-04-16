#ifndef TF02_TRAFFIC_COUNT_H
#define TF02_TRAFFIC_COUNT_H

#include "export.h"
#include <memory>

namespace tf02_common {
class TrafficCountCore;
class API TrafficCount {
 public:
  bool Initialize();
  bool Probe(const double& dist);
  bool Reset();
  int Total() const;
private:
  bool last_probe_;
  int total_;
  std::shared_ptr<TrafficCountCore> core_;
};
} // namespace tf02_common

#endif // TF02_TRAFFIC_COUNT_H
