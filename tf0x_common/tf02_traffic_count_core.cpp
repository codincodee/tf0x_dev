#include "tf02_traffic_count_core.h"
#include <string.h>

#define LIST_MAX_DIST 2200

namespace tf02_common {
#ifdef TRAFFIC_COUNT_CORE_v2
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
#endif

#ifdef TRAFFIC_COUNT_CORE_v1
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
    for(i = 0; i < 100; i++)
    {
        sum += value[i];
    }
    mean_value = int(sum / 100);
    return mean_value;
}

int TrafficCountCore::max(int *value)
{
    int i = 0;
    int max_value = value[0];
    for(i = 0; i < 100; i++)
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
    for(i = 0; i < 100; i++)
    {
        if(min_value > value[i])
        {
            min_value = value[i];
        }
    }
    return min_value;
}

void SwitchingOutput_2(unsigned short dist)
{

}

bool TrafficCountCore::Initialize() {
  threshold = 0;
  k = 0;
  Count_1s = 0;
  Count_60s = 0;
  D = new int[10];
  X = 0;
  T = new int[100];
  STATE = 0;
  DIFF = 0;
  mid_filt_flag = 0;

  return true;
}

TrafficCountCore::~TrafficCountCore() {
  if (D) {
    delete [] D;
  }
  if (T) {
    delete [] T;
  }
}

bool TrafficCountCore::Probe(const double &distance) {
  unsigned short dist = distance * 100.0;
  k++;
  Count_1s++;
  Count_60s++;
  D[k % 10] = dist;

  if(dist == LIST_MAX_DIST)
  {
      int i = 0;
      int find = 0;
      for(i = k % 10; i > -1; i--)
      {
          if(D[i] != 2200)
          {
              D[k % 10] = D[i];
              find = 1;
              break;
          }
      }
      if(mid_filt_flag == 1 && find == 0)
      {
          for(i = 9; i > k % 10; i--)
          {
              if(D[i] != 2200)
              {
                  D[k % 10] = D[i];
                  break;
              }
          }
      }
  }

  if(mid_filt_flag == 1)
  {
      k = k % 10;
      X = median(D);
  }
  else
  {
      X = D[k];
      if(k >= 9)
      {
          mid_filt_flag = 1;
      }
  }

  if(Count_1s > 100)
  {
      Count_1s = 1;
  }
  T[Count_1s - 1] = X;

  if(Count_60s > 6000)
  {
      Count_60s = 1;
  }

  if((threshold == 0 && Count_1s == 100) || (Count_60s == 6000))
  {
      DIFF = max(T) - min(T);
      if(DIFF < 30)
      {
          threshold = mean(T);
      }
  }

  if((threshold != 0) && (X >= (threshold + 50)))
  {
      threshold = 0;
      Count_1s = 0;
      Count_60s = 0;
  }

  if((threshold != 0) && (X <= (threshold - 15)))
  {
      STATE = 1;
  }
  else
  {
      STATE = 0;
  }
  if(STATE == 1)
  {
      return true;
  }
  else if(STATE == 0)
  {
      return false;
  }
}
#endif
} // namespace tf02_common
