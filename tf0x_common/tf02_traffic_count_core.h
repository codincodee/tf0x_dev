#ifndef TF02_TRAFFIC_COUNT_CORE_H
#define TF02_TRAFFIC_COUNT_CORE_H

namespace tf02_common {
class TrafficCountCore {
 public:
  ~TrafficCountCore();
  bool Initialize();
  bool Probe(const double& dist);
 private:
  int median(int* value);
  int mean(int* value);
  int max(int*value);
  int min(int* value);

  int threshold;
  int k;
  int Count_1s;
  int Count_60s;
  int* D;
  int X;
  int* T;
  int STATE;
  int DIFF;
  int mid_filt_flag;
};
} // namespace tf02_common

#endif // TF02_TRAFFIC_COUNT_CORE_H
