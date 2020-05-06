#include "util/base.h"


def(sleep_for) {
  auto ts = nowMs();
  std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(1000));
  ts = nowMs() - ts;
  assert_T(1000 < ts && ts < 1100);

  /*
  // from c++14
  using namespace std::literals::chrono_literals;
  std::this_thread::sleep_for(1s);
  std::this_thread::sleep_for(1000ms);
  */
}


main_run;