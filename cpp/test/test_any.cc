#include "util/base.h"

run(test_Any) {
  Any a(1);

  if (a.type() == typeid(int)) {
    int v = any_cast<int>(a);
    assert_eq(1, v);

    int* ptr = any_cast<int>(&a);
    (*ptr)++;
    assert_eq(2, any_cast<int>(a));

    int& ref = any_cast<int>(a);
    ref++;
    assert_eq(3, any_cast<int>(a));

    try {
      print(any_cast<float>(a));
    } catch (std::exception& e) {
      println("exception: ", e.what());
    }
  }
}

main_run;
