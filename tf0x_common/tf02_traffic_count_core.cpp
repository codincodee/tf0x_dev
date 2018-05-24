#include "tf02_traffic_count_core.h"
#include <string.h>
#include <math.h>
#include <iostream>

#define THRES_LENGTH 500
#define PCL_MEDIAN_ROW_SIZE 30
#define COUNT_1S_MAX 100
#define COUNT_5S_MAX 500
#define COUNT_60S_MAX 2000
#define JUDGE_NUM 100
#define MED_NUM 10
#define INVALID 32760

namespace tf02_common {
  TrafficCountCore::TrafficCountCore() {}
//  : dist_t(nullptr),
//    dist_m_1s(nullptr),
//    dist_m_5s(nullptr),
//    state_5s(nullptr),
//    s_c_5s(nullptr),
//    d_c_5s(nullptr),
//    idxArr(nullptr) {}

bool TrafficCountCore::Initialize() {
//  count = 0;
//  count_1s = 0;
//  count_5s = 0;
//  count_m = 0;
//  count_60s = 0;
//  thres_over_fix = 15;
//  thres_over = 15;
//  thres_renew = 30;
//  thres_step = 100;
//  thres_state = 0;
//  thres_dist = 0;
//  if (dist_t) {
//    delete [] dist_t;
//  }
//  dist_t = new int16_t[MED_NUM]{0};
//  dist_m;
//  if (dist_m_1s) {
//    delete [] dist_m_1s;
//  }
//  dist_m_1s = new int16_t[COUNT_1S_MAX];
//  if (dist_m_5s) {
//    delete [] dist_m_5s;
//  }
//  dist_m_5s = new int16_t[COUNT_5S_MAX];
//  state_now = 0;
//  if (state_5s) {
//    delete [] state_5s;
//  }
//  state_5s = new int16_t[COUNT_5S_MAX];
//  stat_std = 0.0;
//  d_m = 0;
//  d_t = 0;
//  if (s_c_5s) {
//    delete [] s_c_5s;
//  }
//  s_c_5s = new int16_t[COUNT_5S_MAX];
//  if (d_c_5s) {
//    delete [] d_c_5s;
//  }
//  d_c_5s = new int16_t[COUNT_5S_MAX];
//  if (idxArr) {
//    delete [] idxArr;
//  }
//  idxArr = new int16_t[COUNT_5S_MAX];
//  d_t_5s = 0;
//  len_buf = 0;
  return true;
}

TrafficCountCore::~TrafficCountCore() {
//  if (dist_t) {
//    delete [] dist_t;
//  }
//  if (dist_m_1s) {
//    delete [] dist_m_1s;
//  }
//  if (dist_m_5s) {
//    delete [] dist_m_5s;
//  }
//  if (state_5s) {
//    delete [] state_5s;
//  }
//  if (s_c_5s) {
//    delete [] s_c_5s;
//  }
//  if (d_c_5s) {
//    delete [] d_c_5s;
//  }
//  if (idxArr) {
//    delete [] idxArr;
//  }
}

bool TrafficCountCore::Probe(const double &distance) {
  int16_t rawdist = distance * 100;

  static int count = 0;
  static int16_t count_1s = 0;
  static int16_t count_5s = 0;
  static int16_t count_m = 0;
  static int16_t count_60s = 0;
  static int16_t thres_over_fix = 15;
  static int16_t thres_over = 15;
  static int16_t thres_renew = 30;
  static int16_t thres_step = 100;
  static int16_t thres_state = 0;
  static int16_t thres_dist = 0;
  static int16_t dist_t[MED_NUM] = { 0 };
  static int16_t dist_m;
  static int16_t dist_m_1s[COUNT_1S_MAX];
  static int16_t dist_m_5s[COUNT_5S_MAX];
  static int16_t state_now = 0;
  static int16_t state_5s[COUNT_5S_MAX];
  static double stat_std = 0.0;
  static int16_t d_m = 0;
  static int16_t d_t = 0;
  static int16_t s_c_5s[COUNT_5S_MAX];
  static int16_t d_c_5s[COUNT_5S_MAX];
  static int16_t idxArr[COUNT_5S_MAX];
  int16_t stateLen = 0;
  static int16_t d_t_5s = 0;
  int16_t i = 0;
  static int16_t len_buf = 0;

  if (0 == count)
  {
    thres_dist = rawdist;
  }

  //printf("%d\n", count);
  count++;

  count_m = count % MED_NUM;
  count_1s = count % COUNT_1S_MAX;
  count_5s = count % COUNT_5S_MAX;
  count_60s = count % COUNT_60S_MAX;

  if (2200 == rawdist)
  {
    rawdist = 0;
  }
  dist_t[count_m] = rawdist;

//	int16_t testArr[100] = {0};
//	for(i = 0;i <100;i++)
//	{
//		testArr[i] = i;
//	}

//	medianblur_row(testArr, 1, 100);

  if (count >= MED_NUM)
  {
    std::cout << __LINE__ << std::endl;
    std::cout << sizeof(dist_t) << std::endl;
    dist_m = (int16_t)floor((double)median(dist_t, sizeof(dist_t)/2, 0));
    std::cout << __LINE__ << std::endl;
  }
  else
  {
    dist_m = dist_t[count_m];
  }
  dist_m_1s[count_1s] = dist_m;
  dist_m_5s[count_5s] = dist_m;

  if (0 == count_60s || (0 == count_1s && thres_state == 0))
  {
    if (state_now == 0)
    {
      d_m = mean(dist_m_1s, 100);
      d_t = max(dist_m_1s, 100) - min(dist_m_1s, 100);
      if ((d_t < thres_renew) && floor(abs(d_m - thres_dist) < thres_step))
      {
        thres_dist = d_m;
        thres_state = 1;
      }
      else
      {
        thres_state = 0;
      }
    }
    else
    {
      thres_state = 0;
    }
  }
  if (0 == count_5s)
  {
    for (i = 0; i < COUNT_5S_MAX; i++)   //count---N/A
    {
      idxArr[i] = state_5s[i];
      s_c_5s[i] = state_5s[i];
    }
    for (i = 0; i < COUNT_5S_MAX; i++)
    {
      d_c_5s[i] = dist_m_5s[i];
    }
    stateLen = COUNT_5S_MAX - 1;
    diffArr(idxArr, COUNT_5S_MAX, 0);
    len_buf = stateLen;
    stateLen = findArr(idxArr, len_buf); // OK
    len_buf = stateLen;
    if(stateLen != 0)
    {
      idxArr[COUNT_5S_MAX - 1] = INVALID;
      int16_t offset = 0, idxValue = 0;
      for (offset = -10; offset <= 10; offset++)
      {
        for (i = 0; i <= stateLen; i++)
        {
          if (idxArr[i] != INVALID)
          {
            idxValue = idxArr[i] + offset;
            if (idxValue >= 0 && idxValue < COUNT_5S_MAX-1)
            {
              d_c_5s[idxValue+1] = INVALID;
            }
          }
        }
      }
    }

    removeData(d_c_5s, s_c_5s, sizeof(s_c_5s) / 2, 1);
    removeData(d_c_5s, d_c_5s, sizeof(d_c_5s) / 2, 0);
    d_c_5s[0] = INVALID;//ok
    d_c_5s[COUNT_5S_MAX-1] = INVALID;//ok
    int j = 0, length_medfilt = 0;
    for (i = 0; i < COUNT_5S_MAX; i++)
    {
      if (d_c_5s[i] != INVALID)
      {
        d_c_5s[j] = d_c_5s[i];
        j++;
      }

    }
    length_medfilt = j;
    if (length_medfilt > 30)
    {
      d_t_5s = max(d_c_5s, length_medfilt) - min(d_c_5s, length_medfilt);//ok
      if (d_t_5s < 50)
      {
        medianblur_row(d_c_5s, 1, length_medfilt);
        stat_std = stdArrErr(d_c_5s, length_medfilt);
        //printf("stat_std = %.2f\r\n",stat_std);
        if (stat_std < 1)
        {
          stat_std = 0.0;
        }
        else if (stat_std < 2)
        {
          stat_std = 5;
        }
        else if (stat_std < 3)
        {
          stat_std = 10;
        }
        else if (stat_std < 5)
        {
          stat_std = 30;
        }
        else
        {
          stat_std = 50;
        }
      }
    }
  }

  thres_over = thres_over_fix + stat_std;
  if (count > JUDGE_NUM)
  {
    if (thres_dist - dist_m > thres_over)
    {
      state_now = 1;
    }
    else
    {
      state_now = 0;
    }
  }
  state_5s[count_5s] = state_now;

  if(state_now == 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int16_t TrafficCountCore::median(int16_t *value, uint8_t length, uint8_t startIndex)
{
  int i, j;
  int16_t temp;
  int mid_filt_value = 0;
  int16_t value_med[30] = {0};

    for (i = 0; i < length; i++)
    {
      if (value[i] != INVALID)
      {
        value_med[i] = value[i];
      }
    }

  if (0 == length)
  {
    return 0;
  }
  for (i = startIndex; i < length - 1; ++i)
  {
    for (j = startIndex; j < length - i - 1; ++j)
    {
      if (value_med[j] > value_med[j + 1])
      {
        temp = value_med[j];
        value_med[j] = value_med[j + 1];
        value_med[j + 1] = temp;
      }
    }
  }
  //odd
  if (length & 0x1)
  {
    mid_filt_value = value_med[length / 2];
  }
  else    //even
  {
    mid_filt_value = (value_med[length / 2 - 1] + value_med[length / 2]) / 2;
  }
  return mid_filt_value;
}

int16_t TrafficCountCore::mean(int16_t *value, int length)
{
  int i = 0;
  long long sum = 0;
  int mean_value = 0;

    for (i = 0; i < length; i++)
    {
      if (value[i] != INVALID)
      {
      sum += value[i];
      }
    }
    mean_value = sum / length;
  //	printf("--------------mean_value = %d\r\n",mean_value);
  return mean_value;
}

int16_t TrafficCountCore::max(int16_t *value, int length)
{
  int i = 0;

  int16_t max_value = value[0];

    for (i = 0; i < length; i++)
    {
      if (value[i] != INVALID)
      {
      if (max_value < value[i])
      {
        max_value = value[i];
      }
      }
    }
  //	printf("--------------max = %d\r\n",max_value);
  return max_value;
}

int16_t TrafficCountCore::min(int16_t *value, int length)
{
  int i = 0;
  int16_t min_value = value[0];

    for (i = 0; i < length; i++)
    {
      if (value[i] != INVALID)
      {
      if (min_value > value[i])
      {
        min_value = value[i];
      }
    }
  }
  //	printf("--------------min_value = %d\r\n",min_value);
  return min_value;
}

void TrafficCountCore::diffArr(int16_t *value, int length, int startIndex)
{
  int i = 0, j = 0;
  for (i = startIndex; i < length-1; i++)
  {
    if (value[i] != INVALID)
    {
    j = i + 1;
    value[i] = value[j] - value[i];
    }
  }
}

int16_t TrafficCountCore::findArr(int16_t *value, int16_t length)
{
  int i = 0, j = 0;
  for (i = 0; i < length; i++)
  {
    if (value[i] != INVALID)
    {
    if (value[i] != 0)
    {
      value[j] = i;
      j++;
    }
    }
  }
  for (i = j; i < length; i++)
  {
    value[i] = INVALID;
  }
  length = j;
  return length;
}

void TrafficCountCore::removeData(int16_t *value, int16_t *marker, int length, int data)
{
  int i = 0;
  for (i = 0; i < length; i++)
  {
    if (value[i] != INVALID)
    {
    if (data == marker[i])
    {
      value[i] = INVALID;
    }
    }
  }
}

int16_t TrafficCountCore::medianblur_row(int16_t *distance, int row, int colum)
{
  int j,k;
  int16_t data[PCL_MEDIAN_ROW_SIZE];
  int16_t distance_copy[500];

  for(k = 0;k<colum;k++)
  {
    if (distance[k] != INVALID)
    {
      distance_copy[k] = distance[k];
    }
  }

  if (NULL == distance)
  {
    return -1;
  }

    for (j = 0; j < colum ; ++j)
    {

      if (j < PCL_MEDIAN_ROW_SIZE / 2)
      {
        for(k = 0; k< (PCL_MEDIAN_ROW_SIZE/2 + j);k++)
        {
           data[k] = distance_copy[k];
        }
      //	memcpy(data, &distance[0], (PCL_MEDIAN_ROW_SIZE/2 + j) * sizeof(int16_t));
        for (k = (PCL_MEDIAN_ROW_SIZE / 2 + j); k < PCL_MEDIAN_ROW_SIZE; k++)
        {
          data[k] = 0;
        }
      }
      else if (j >= (colum - PCL_MEDIAN_ROW_SIZE / 2))
      {
        for(k = 0; k< (colum - j + PCL_MEDIAN_ROW_SIZE / 2);k++)
        {
           data[k] = distance_copy[j - PCL_MEDIAN_ROW_SIZE / 2 +k];
        }
        //memcpy(data, &distance[j - PCL_MEDIAN_ROW_SIZE / 2], (colum - j + PCL_MEDIAN_ROW_SIZE / 2) * sizeof(int16_t));
        for (k = (colum - j + PCL_MEDIAN_ROW_SIZE/2); k < PCL_MEDIAN_ROW_SIZE; k++)
        {
          data[k] = 0;
        }
      }
      else
      {
        for(k = 0 ; k< PCL_MEDIAN_ROW_SIZE;k++)
        {
           data[k] = distance_copy[j - PCL_MEDIAN_ROW_SIZE / 2 +k];
        }
        //memcpy(data, &distance[j - PCL_MEDIAN_ROW_SIZE / 2], PCL_MEDIAN_ROW_SIZE * sizeof(int16_t));
      }
      distance[j] = median(data, PCL_MEDIAN_ROW_SIZE, 0);
      //distance_copy[j] = distance[j];
  }
    distance[0] = INVALID;


  return 0;
}

double TrafficCountCore::stdArrErr(int16_t *array, int length)
{
  double stdarrerr = 0.0;

  int16_t i = 0;
  double sum = 0.0,averageArr = 0.0;
  double sumCorr = 0.0;
  int16_t backArr[500] = {0};
  for( i =0;i<length;i++)
  {
    backArr[i] = array[i];
  }

    for (i = 0; i < length; i++)
    {
      if (backArr[i] != INVALID)
      {
        sum += backArr[i];
      }

    }
    averageArr = sum / (length-1);

    for (i = 0; i < length; i++)
    {
      if (backArr[i] != INVALID)
      {
        sumCorr += (double)(backArr[i] - averageArr) * (double)(backArr[i] - averageArr);
      }
    }

    stdarrerr = sqrt((double)sumCorr/(length-1));
  return stdarrerr;
}
} // namespace tf02_common
