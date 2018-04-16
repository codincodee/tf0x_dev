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

bool TrafficCountCore::Probe(const double &dist) {
  k++;
  Count_1s++;
  Count_60s++;
  D[k % 10] = unsigned short(dist * 100.0);

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
} // namespace tf02_common
