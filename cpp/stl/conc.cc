#include "util/base.h"


def(mutex_lock) {
  const int64_t total = 10000;
  int64_t cnt = 0;

  std::mutex mtx;
  auto add = [&cnt, &mtx](int64_t n) {
    while (--n >= 0) {
      std::lock_guard<std::mutex> lk(mtx);
      cnt++;
    }
  };

  const int64_t th_num = 1000;
  vector<thread> ths;
  for (int64_t i = 0; i < th_num; i++) ths.emplace_back(add, total);
  for (auto& th : ths) th.join();
  assert_eq(total*th_num, cnt);
}


main_run;