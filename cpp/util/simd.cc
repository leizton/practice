#ifdef __AVX__
  #include <immintrin.h>
#else
  #warning No AVX support - will not compile
#endif

#include <algorithm>
#include <cmath>

#include "avx_math.h"

void __softmax(int num, const float* src, float* dst) {
  if (num <= 0) { return; }
  if (num == 1) { *dst = 1; return; }

  float max_value = src[0];
  for (int i = 1; i < num; i++) {
    max_value = std::max(max_value, src[i]);
  }

  float tmp = 0, exp_sum = 0;
  for (int i = 0; i < num; i++) {
    tmp = std::exp(std::max(src[i] - max_value, -80.f));
    dst[i] = tmp;
    exp_sum += tmp;
  }
  for (int i = 0; i < num; i++) {
    dst[i] /= exp_sum;
  }
}

void avx_softmax(int num, const float* src, float* dst) {
  if (num < 16) {
    __softmax(num, src, dst);
    return;
  }
  int batch  = num / 8;
  float tmp[8];

  // step-1: compute max_value
  // unit_val : eight local max_values in PU
  __m256 unit_val = _mm256_loadu_ps(src);
  for (int i = 1; i < batch; i++) {
    unit_val = _mm256_max_ps(unit_val, _mm256_loadu_ps(src + i*8));
  }
  unit_val = _mm256_max_ps(unit_val, _mm256_loadu_ps(src + num - 8));
  // compute gloal max_value
  _mm256_storeu_ps(tmp, unit_val);
  float max_value = tmp[0];
  for (int i = 1; i < 8; i++) max_value = std::max(max_value, tmp[i]);

  // step-2: compute exp(xi-max_v) / sum(exp(Xi-max_v))
  __m256 max_value_m256 = _mm256_set1_ps(max_value);  // _mm256_set1_ps用一个值填充向量寄存器
  __m256 x_min_m256 = _mm256_set1_ps(-80.f);
  __m256 unit_sum = _mm256_set1_ps(0.f);
  for (int i = 0; i < batch; i++) {
    // exp(xi - max_v)
    unit_val = _mm256_sub_ps(_mm256_loadu_ps(src + i*8), max_value_m256);
    unit_val = _mm256_max_ps(unit_val, x_min_m256);
    unit_val = exp256_ps(unit_val);
    _mm256_storeu_ps(dst + i*8, unit_val);
    //
    unit_sum = _mm256_add_ps(unit_sum, unit_val);
  }
  _mm256_storeu_ps(tmp, unit_sum);
  float sum_exp = tmp[0];
  for (int i = 1; i < 8; i++) sum_exp += tmp[i];

  int remain = num % 8;
  const float* src_remain_begin = src + (batch * 8);
  float* dst_remain_begin = dst + (batch * 8);
  for (int i = 0; i < remain; i++) {
    float exp_v = std::exp(std::max(src_remain_begin[i] - max_value, -80.f));
    dst_remain_begin[i] = exp_v;
    sum_exp += exp_v;
  }

  __m256 sum_exp_m256 = _mm256_set1_ps(sum_exp);
  for (int i = 0; i < batch; i++) {
    // using `1 / ((1/x) * sum)` instead `x/sum` or `x * (1/sum)`
    unit_val = _mm256_rcp_ps(_mm256_loadu_ps(dst + i*8));  // calc (1/x). rcp计算近似倒数
    unit_val = _mm256_rcp_ps(_mm256_mul_ps(unit_val, sum_exp_m256));
    _mm256_storeu_ps(dst + i*8, unit_val);
  }
  unit_val = _mm256_rcp_ps(_mm256_loadu_ps(dst + num - 8));
  unit_val = _mm256_rcp_ps(_mm256_mul_ps(unit_val, sum_exp_m256));
  _mm256_storeu_ps(dst + num - 8, unit_val);
}
