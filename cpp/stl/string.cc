#include "util/base.h"


def(find_rfind) {
  string s = "a/b/c/d";
  assert_eq(std::string::npos, s.find('\\'));
  assert_eq(1, s.find('/'));
  assert_eq(3, s.find('/', 2));
  assert_eq(5, s.rfind('/'));
}


def(starts_with) {
  auto starts_with = [](const string& s, const string& prefix) {
    return s.find(prefix) == 0u;
  };
  string s = "12345";
  assert_T(starts_with(s, "123"));
  assert_T(starts_with(s, ""));
  assert_F(starts_with(s, "abc"));
  assert_F(starts_with(s, "123456"));
}


def(dirpath) {
  auto getDirpath = [] (const string& path) -> string {
    auto idx = path.rfind('/');
    return (idx == std::string::npos) ? "" : path.substr(0, idx+1);
  };
  assert_eq("/home/", getDirpath("/home/a.txt"));
  assert_eq("/home/lz/", getDirpath("/home/lz/a.txt"));
  assert_eq("", getDirpath("a.txt"));
}


def(basic_string_deep_copy) {
  const int N = 3;
  TrivialInt arr[N];
  for (int i = 0; i < N; i++) arr[i].v = 0;

  // arr到s是深拷贝
  basic_string<TrivialInt> s(arr, arr+3);
  assert_eq(3, s.size());
  assert_neq(arr, s.data());

  assert_eq(arr[1].v, s[1].v);
  s[1].v = 10;
  assert_neq(arr[1].v, s[1].v);
}


main_run;