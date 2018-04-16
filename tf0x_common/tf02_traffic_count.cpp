#include "tf02_traffic_count.h"
#include "tf02_traffic_count_core.h"

namespace tf02_common {
bool TrafficCount::Initialize() {
  return Reset();
}

bool TrafficCount::Probe(const double &dist) {
  auto probe = core_->Probe(dist);
  if (last_probe_ && !probe) {
    ++total_;
  }
  last_probe_ = probe;
  return probe;
}

bool TrafficCount::Reset() {
  total_ = 0;
  last_probe_ = false;
  core_.reset(new TrafficCountCore);
  return core_->Initialize();
}

int TrafficCount::Total() const {
  return total_;
}
} // namespace tf02_common
