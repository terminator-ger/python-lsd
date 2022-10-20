#include "../include/statistics.h"
#include <iostream>
#include <random>
#include <stdlib.h>
#include <string.h>


static void error(char * msg)
{
  fprintf(stderr,"LSD Error: %s\n",msg);
  exit(EXIT_FAILURE);
}

float sstats::max(const float* list, const int size){
  float max_ = 0;
  for(int i=0; i<size; ++i){
    if (list[i] > max_){
      max_ = list[i];
    }
  }
  return max_;
}


float sstats::min(const float* list, const int size){
  float min_ = std::numeric_limits<float>::max();
  for(int i=0; i<size; ++i){
    if (list[i] < min_){
      min_ = list[i];
    }
  }
  return min_;
}

sstats::MINMAX sstats::minmax(const float* list, const int size){
  float min_ = std::numeric_limits<float>::max();
  float max_ = 0;
  for(int i=0; i<size; ++i){
    if (list[i] < min_){
      min_ = list[i];
    }
    if (list[i] > max_){
      max_ = list[i];
    }
  }
  return sstats::MINMAX(min_, max_);
}



float sstats::mean(const float* arr, const int size){
  float sum=0.;
  for(int i=0;i<size;++i){
    sum += arr[i];
  }
  float mean = (sum/size);
  return mean;
}

float sstats::std(const float* arr, const int size){
  float mean = sstats::mean(arr, size);
  float var=0.;
  for(int i=0;i<size;++i){
    var += (arr[i]-mean)*(arr[i]-mean);
  }
  var /= (size-1);
  return var;
}



sstats::M_STD sstats::mean_std(const float* arr, const int size){
  float sum=0.;
  for(int i=0;i<size;++i){
    sum += arr[i];
  }
  float mean = (sum/size);
  float var=0.;
  for(int i=0;i<size;++i){
    var += (arr[i]-mean)*(arr[i]-mean);
  }
  var /= (size-1);
  return sstats::M_STD(mean, var);
}



float sstats::median(const float* list, const int size){
  // local copy of list
    float* cpy_list = new float[size];
    for(int i=0; i<size; ++i){
      cpy_list[i] = list[i];
      //std::cout << cpy_list[i] << std::endl;
    }

    if (size % 2 == 1){
        return sstats::__quickselect(cpy_list, size, size/2);
    }else{
        float * cpy_list_2 = new float[size];
        memcpy(cpy_list_2, cpy_list, size*sizeof(float));
        float m1 = sstats::__quickselect(cpy_list, size, size/2-1);
        float m2 = sstats::__quickselect(cpy_list_2, size, size/2);
        return (m1+m2)/2;
    }
}

float sstats::__quickselect(float* list, const int size, const int k){
    if (size <= 5){
        float val = sstats::__nlogn_median(list, size);
        free(list);
        return val;
    }
    sstats::MINMAX minmax = sstats::minmax(list, size);
    float rnd =  ((float)rand())/(RAND_MAX+1.);
    float pivot = rnd * (minmax.max-minmax.min) + minmax.min;

    float* lows   = new float[size];
    float* pivots = new float[size];
    float* highs  = new float[size];
    int low_size = 0;
    int piv_size = 0;
    int hig_size = 0;

    for (int i = 0; i<size; ++i){
        if (list[i] < pivot){
            lows[low_size] = list[i];
            ++low_size;
        }else if (list[i] == pivot){
            pivots[piv_size] = list[i];
            ++piv_size;
        }else{
            highs[hig_size] = list[i];
            ++hig_size;
        }
    }
    free(list);

    if (k < low_size){
      free(highs);
      free(pivots);
      return sstats::__quickselect(lows, low_size, k);
    }
    else if (k < low_size+piv_size){
        float ret = pivots[0];
        free(highs);
        free(lows);
        free(pivots);
        return ret;
    }else{
      free(lows);
      free(pivots);
        return sstats::__quickselect(highs, hig_size, k-low_size-piv_size);
    }

}

int sstats::__compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}


float sstats::__nlogn_median(float* list, const int size){
  qsort(list, size, sizeof(float), sstats::__compare);

  if (size % 2 == 1){
    return list[size/2];
  }else{
    return 0.5* (list[size/2-1] + list[size/2]);
  }
}
