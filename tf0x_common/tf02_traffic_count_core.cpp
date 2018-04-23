#include "tf02_traffic_count_core.h"
#include <string.h>

#define LIST_MAX_DIST 2200

namespace tf02_common {

int TrafficCountCore::median(int *value)
{
    int tmpValue[10];
    int i = 0, j = 0;
    int mid_filt_value = 0;

    memcpy((void *)&tmpValue, (void *)value, 10 * sizeof(int));

    for(i = 0; i < 10 - 1; i++)
    {
        for(j = i; j < 10 - 1; j++)
        {
            int tmp;
            if(tmpValue[i] > tmpValue[j + 1])
            {
                tmp = tmpValue[j + 1];
                tmpValue[j + 1] = tmpValue[i];
                tmpValue[i] = tmp;
            }
        }
    }
    mid_filt_value = tmpValue[5];
    return mid_filt_value;
}

int TrafficCountCore::mean(int *value)
{
    int i = 0;
    long long sum = 0;
    int mean_value = 0;
    for(i = 0; i < 50; i++)
    {
        sum += value[i];
    }
    mean_value = sum / 50;
    return mean_value;
}

int TrafficCountCore::max(int *value)
{
    int i = 0;
    int max_value = value[0];
    for(i = 0; i < 50; i++)
    {
        if(max_value < value[i])
        {
            max_value = value[i];
        }
    }
    return max_value;
}

int TrafficCountCore::min(int *value)
{
    int i = 0;
    int min_value = value[0];
    for(i = 0; i < 50; i++)
    {
        if(min_value > value[i])
        {
            min_value = value[i];
        }
    }
    return min_value;
}

bool TrafficCountCore::Initialize() {
  k = 0;
  count_cont=0;
  flag_exceed=0;
  threshold=0;
  threshold_last=0;
  state=0;
  state_last=0;
  dist_buf = new int[10]{0};
  dist_med=0;
  dist_cnt = new int[50]{0};
  count_buf= 0;
  count_fix= 0;
  idx_buf= 0;
  mid_filt_flag = 0;
  return true;
}

TrafficCountCore::~TrafficCountCore() {
  if (dist_buf) {
    delete [] dist_buf;
    dist_buf = nullptr;
  }

  if (dist_cnt) {
    delete [] dist_cnt;
    dist_cnt = nullptr;
  }
}

bool TrafficCountCore::Probe(const double& dist)
{
  unsigned short corrdist = dist * 100.0;

  count_cont+=1;
  k++;
  count_buf = k % 50;
  count_fix = k % 300;
  idx_buf   = k % 10;
  dist_buf[idx_buf] = corrdist;


  if(dist_buf[idx_buf] == 2200)
  {
    int i = 0;
    int find = 0;
    for(i = k % 10; i > -1; i--)
    {
      if(dist_buf[i] != 2200)
      {
        dist_buf[k % 10] = dist_buf[i];
        find = 1;
        break;
      }
    }
    if(mid_filt_flag == 1 && find == 0)
    {
      for(i = 9; i > k % 10; i--)
      {
        if(dist_buf[i] != 2200)
        {
          dist_buf[k % 10] = dist_buf[i];
          break;
        }
      }
    }
  }

    if(mid_filt_flag == 1)
    {
    k = k % 300;
    state = state_last;
    threshold = threshold_last;
    dist_med = median(dist_buf);
    }
    else
    {
    state = 0;
    threshold = 0;
    dist_med = dist_buf[idx_buf];
        if(k >= 9)
        {
            mid_filt_flag = 1;
        }
    }

  dist_cnt[count_buf] = dist_med;

  if((dist_med > threshold + 5) && (threshold != 0))
  {
    flag_exceed = 1;
  }

  if((flag_exceed == 1) || (threshold == 0 && count_buf == 49) || (count_fix == 299))
  {
    if(((max(dist_cnt) - min(dist_cnt)) < 5) && (mean(dist_cnt) > (threshold - 50)))
    {
      threshold = mean(dist_cnt);
      flag_exceed = 0;
    }
  }

  if(threshold != 0)
  {
    if(count_cont >= 100)
    {
      if(dist_med <= (threshold - 15))
      {
        state = 1;
      }
      else
      {
        state = 0;
      }
    }
    else
    {
      state = state_last;
    }
  }

  if(state != state_last)
  {
    count_cont = 0;
  }

  state_last = state;
  threshold_last = threshold;

  if(state == 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}
} // namespace tf02_common
