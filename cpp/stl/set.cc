#include "util/base.h"


run(add) {
  unordered_set<string> s;
  s.insert("a");
  s.emplace("b");  // 直接调用构造函数
  assert_T(s.find("a") != s.end());
  assert_T(s.find("b") != s.end());
  assert_T(s.find("c") == s.end());
}


main_run;