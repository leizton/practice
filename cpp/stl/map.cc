#include "util/base.h"


def(get_or_create) {
  map<int, Any> m;
  const int key = 1;

  Any* val = nullptr;
  auto it = m.find(key);
  if (it == m.end()) {
    pair<map<int, Any>::iterator, bool> new_pair = m.insert({key, Any(0)});
    val = &new_pair.first->second;
  } else {
    val = &it->second;
  }

  int& pval = any_cast<int>(*val);
  pval++;
  assert_eq(1, any_cast<int>(m.find(key)->second));
}


def(emplace_insert) {
  const std::string key = "a";
  map<string, string> a;
  std::pair<decltype(a)::iterator, bool> ret;

  string s = "123";
  ret = a.emplace(key, s);
  assert_T(ret.second);
  assert_eq("123", a[key]);

  // emplace insert 重复插入时不会覆盖
  s = "456";
  ret = a.emplace(key, s);
  assert_F(ret.second);
  assert_eq("123", a[key]);

  ret = a.insert({key, s});
  assert_F(ret.second);
  assert_eq("123", a[key]);

  // operator[] 会覆盖
  a[key] = s;
  assert_eq("456", a[key]);
}


def(emplace_insert_2) {
  Aoo::log = false;

  // emplace: 1次构造 1次析构
  {
    map<int, Aoo> a;
    a.emplace(1, 0);
    assert_eq(1, Aoo::con());
    assert_eq(0, Aoo::copy_con());
    assert_eq(0, Aoo::move_con());
  }
  assert_eq(1, Aoo::decon());
  Aoo::reset();

  // insert: 1次构造 2次移动构造 3次析构
  {
    map<int, Aoo> a;
    a.insert({1, Aoo()});
    assert_eq(1, Aoo::con());
    assert_eq(0, Aoo::copy_con());
    assert_eq(2, Aoo::move_con());
  }
  assert_eq(3, Aoo::decon());
  Aoo::reset();

  // insert by pair: 1次构造 1次拷贝构造 1次移动构造 3次析构
  {
    // 创建pair: 1次构造 1次移动构造
    pair<int, Aoo> p{1, Aoo()};
    assert_eq(1, Aoo::con());
    assert_eq(0, Aoo::copy_con());
    assert_eq(1, Aoo::move_con());

    // 增加 1次拷贝构造
    map<int, Aoo> a;
    a.insert(p);
    assert_eq(1, Aoo::con());
    assert_eq(1, Aoo::copy_con());
    assert_eq(1, Aoo::move_con());
  }
  assert_eq(3, Aoo::decon());
  Aoo::reset();
}


def(erase) {
  unordered_map<string, int> a;
  a["a"] = 97;
  a["b"] = 98;
  a["c"] = 99;
  a["d"] = 100;

  a.erase(a.find("d"));
  assert_T(a.find("d") == a.end());

  assert_F(a.find("b") == a.end());
  for (auto iter = a.begin(); iter != a.end(); ) {
    if (iter->first == "b")
      iter = a.erase(iter);
    else
      ++iter;
  }
  assert_T(a.find("b") == a.end());
  assert_eq(2, a.size());
}


main_run;