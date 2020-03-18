#include "util/base.h"


def(get_or_create) {
  map<int, Any> m;
  const int key = 1;

  Any* val = nullptr;
  auto it = m.find(key);
  if (it == m.end()) {
    auto ret = m.insert({key, Any(0)});
    val = &ret.first->second;
  } else {
    val = &it->second;
  }

  int& pval = any_cast<int>(*val);
  pval++;
  assert_eq(1, any_cast<int>(m.find(key)->second));
}


main_run;