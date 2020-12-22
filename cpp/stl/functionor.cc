#include "util/base.h"


run(test) {
  function<void()> fn = []() {
    print("test");
  };
  fn();
}


main_run;