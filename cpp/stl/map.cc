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


def(emplace_insert) {
  map<string, string> a;
  string s = "123", key = "a";
  a.emplace(key, s);
  assert_eq("123", a[key]);

  // emplace insert 重复插入时不会覆盖
  s = "456";
  a.emplace(key, s);
  assert_eq("123", a[key]);
  a.insert({key, s});
  assert_eq("123", a[key]);

  // operator[] 会覆盖
  a[key] = s;
  assert_eq("456", a[key]);
}


main_run;