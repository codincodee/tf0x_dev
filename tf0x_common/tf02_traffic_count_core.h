#ifndef TF02_TRAFFIC_COUNT_CORE_H
#define TF02_TRAFFIC_COUNT_CORE_H

#include <cstdint>

namespace tf02_common {
class TrafficCountCore {
 public:
  TrafficCountCore();
  ~TrafficCountCore();
  bool Initialize();
  bool Probe(const double& dist);
 private:
  int16_t median(int16_t *value, uint8_t length, uint8_t startIndex);
  int16_t mean(int16_t *value, int length);
  int16_t max(int16_t *value, int length);
  int16_t min(int16_t *value, int length);
  void diffArr(int16_t *value, int length, int startIndex);
  int16_t findArr(int16_t *value, int16_t length);
  void removeData(int16_t *value, int16_t *marker, int length, int data);
  int16_t medianblur_row(int16_t *distance, int row, int colum);
  double stdArrErr(int16_t *array, int length);

//  int count;
//  int16_t count_1s;
//  int16_t count_5s;
//  int16_t count_m;
//  int16_t count_60s;
//  int16_t thres_over_fix;
//  int16_t thres_over;
//  int16_t thres_renew;
//  int16_t thres_step;
//  int16_t thres_state;
//  int16_t thres_dist;
//  int16_t* dist_t;
//  int16_t dist_m;
//  int16_t* dist_m_1s;
//  int16_t* dist_m_5s;
//  int16_t state_now;
//  int16_t* state_5s;
//  double stat_std;
//  int16_t d_m;
//  int16_t d_t;
//  int16_t* s_c_5s;
//  int16_t* d_c_5s;
//  int16_t* idxArr;
//  int16_t d_t_5s;
//  int16_t len_buf;
};
} // namespace tf02_common

#endif // TF02_TRAFFIC_COUNT_CORE_H
