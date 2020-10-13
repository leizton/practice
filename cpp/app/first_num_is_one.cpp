#include "util/base.h"

int getFirstNum(int x) {
  while (1) {
    int t = x / 10;
    if (t == 0) return x;
    x = t;
  }
  return 0;
}

/*
1, 241
2, 312
3, 358
4, 348
5, 328
6, 307
7, 287
8, 270
9, 254
*/
run() {
  const int N = 10000;

  for (int k = 1; k < 10; k++) {
    int max = k * N;
    int64_t sum = 0;
    int n = 0;
    for (int i = 1; i < max; i++) {
      if (getFirstNum(i) == 1) n++;
      int r = 1000 * n / i;
      sum += r;
    }
    print(k, sum / max);
  }
}

main_run;