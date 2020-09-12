#include "util/base.h"

def(genRand) {
  /**
   * 线性同余生成伪随机数
   * X[n+1] = a * X[n] + c
   */
  const uint32_t a = 1103515245;
  const uint32_t c = 12345;
  for (uint32_t start = 0; start < 5; start++) {
    uint32_t ran = start;
    map<int, int> st;
    for (int i = 0; i < 1000; i++) {
      st[ran % 5]++;
      ran = (a * ran + c);
    }
    vector<int> cnt;
    for (auto& e : st) {
      cnt.push_back(e.second);
    }
    print(cnt);
  }
}

main_run;
