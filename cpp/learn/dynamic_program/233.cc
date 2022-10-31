#include <header>

/*
给定一个整数 n, 计算所有小于等于 n 的非负整数中数字 1 出现的个数
*/

/*
以百位举例, 设 n = ...abc
以下计算a等于1的次数
⒈ 千位上的变化使百位经过 000~999 的多轮周期, 每轮中的100~199是百位1数字, 共有100个。 所以有 ceil(n/1000)*100
⒉ abc=n%1000.  当abc<100 百位1共0个;  当100<=abc<=199 百位1共有abc-100+1;  当199<a 百位1共有100个。 所以有 min(max(n%1000-100+1, 0), 100)
*/

int countDigitOne(int64_t n) {
  int64_t p = 1, q = 1;
  int num = 0;
  for (int k = 1; true; k++) {
    if (n < p) break;
    q = p;
    p *= 10;
    num += ceil(n / p) * q;
    num + min(max((n%p) - q + 1, int64_t(0)), q);
  }
  return num;
}
