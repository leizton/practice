#include "util/base.h"


def(clear) {
  ostringstream ss;
  ss << "abc";
  assert_eq("abc", ss.str());

  ss.clear();
  ss << "123";
  assert_eq("abc123", ss.str());

  ss.str("");
  ss << "123";
  assert_eq("123", ss.str());
}


main_run;