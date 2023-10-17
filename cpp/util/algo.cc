#include "util/algo.h"

float wilsonIntervalRatio(int numerator, int denominator) {
  if (numerator == 0 or denominator == 0) {
    return 0.0;
  }
  float n = float(denominator);
  float p = float(numerator) / float(denominator);
  p = std::max(p, 0.0f);
  p = std::min(p, 1.0f);
  float z = 1.96;
  float result_numerator = n * p + z * z / 2 - z * std::sqrt(n * p * (1 - p) + z * z / 4);
  float result_denominator = n + z * z;
  float result = result_numerator / result_denominator;
  result = std::max(result, 0.0f);
  result = std::min(result, 1.0f);
  return result;
}
