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

  int k;
  int count_cont;
  int flag_exceed;
  int threshold;
  int threshold_last;
  int state;
  int state_last;
  int* dist_buf = nullptr;
  int dist_med;
  int* dist_cnt = nullptr;
  int count_buf;
  int count_fix;
  int idx_buf;
  int mid_filt_flag;
};
} // namespace tf02_common

#endif // TF02_TRAFFIC_COUNT_CORE_H
