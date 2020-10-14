#include "util/base.h"


def(thread__) {
  auto square = [](int x) {
    print(x * x);
  };
  vector<thread> ths;
  for (int i = 1; i <= 3; i++) {
    ths.emplace_back(square, i);
  }
  for (auto& th : ths) {
    th.join();
  }
}


def(mutex__) {
  std::mutex mtx;
  int n = 0;
  {
    std::lock_guard<std::mutex> lk(mtx);
    n++;
  }
}


main_run;