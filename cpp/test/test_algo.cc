#include <util/base.h>

#define STR_VEC(...) std::vector<std::string>{__VA_ARGS__}

void splitStringTest() {
  std::vector<std::string> res;
  splitString("", ",", res);
  assert_eq(STR_VEC(""), res);
  splitString("ab", ";;", res);
  assert_eq(STR_VEC("ab"), res);
  splitString("a;;b", ";;", res);
  assert_eq(STR_VEC("a", "b"), res);
  splitString("a;;b;;", ";;", res);
  assert_eq(STR_VEC("a", "b", ""), res);
  splitString(";;a;;b;;", ";;", res);
  assert_eq(STR_VEC("", "a", "b", ""), res);
  splitString(";;", ";;", res);
  assert_eq(STR_VEC("", ""), res);
  splitString(";;;;", ";;", res);
  assert_eq(STR_VEC("", "", ""), res);
  splitString(";;;;;;", ";;", res);
  assert_eq(STR_VEC("", "", "", ""), res);
}

int main() {
  splitStringTest();
}
