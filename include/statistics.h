#ifndef STATISTICS__
#define STATISTICS__

namespace sstats{

struct M_STD{
  M_STD(float m, float s):mean(m), std(s){};
  float mean;
  float std;
};
M_STD mean_std(const float* arr, const int size);
M_STD median_std(const float* arr, const int size);

float std(const float* dist, const int size);
float median(const float* dist, const int size);
float mean(const float* dist, const int size);


struct MINMAX{
  MINMAX(float min, float max):min(min),max(max){};
  float min;
  float max;
};
float min(const float* dist, const int size);
float max(const float* dist, const int size);
MINMAX minmax(const float* dist, const int size);



float __quickselect(float* list, const int size, int k);
float __nlogn_median(float* list, const int size);
int __compare(const void* a, const void*b);
}

#endif
