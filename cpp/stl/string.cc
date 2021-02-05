#include "util/base.h"

def(find_rfind) {
  string s = "a/b/c/d";
  assert_eq(std::string::npos, s.find('\\'));
  assert_eq(1u, s.find('/'));
  assert_eq(3u, s.find('/', 2));
  assert_eq(5u, s.rfind('/'));
}

def(starts_with_ends_with) {
  auto starts_with = [](const string& s, const string& prefix) {
    if (s.length() < prefix.length()) return false;
    return strncmp(s.c_str(), prefix.c_str(), prefix.length()) == 0;
  };

  auto ends_with = [](const string& s, const string& prefix) {
    const size_t n = prefix.length();
    if (s.length() < n) return false;
    return strncmp(s.c_str() + (s.length() - n), prefix.c_str(), n) == 0;
  };

  string s = "12345";
  assert_T(starts_with(s, "123"));
  assert_T(starts_with(s, ""));
  assert_F(starts_with(s, "abc"));
  assert_F(starts_with(s, "123456"));

  assert_T(ends_with(s, "345"));
  assert_T(ends_with(s, ""));
  assert_F(ends_with(s, "abc"));
  assert_F(ends_with(s, "012345"));
}

def(dirpath) {
  auto getDirpath = [](const string& path) -> string {
    auto idx = path.rfind('/');
    return (idx == std::string::npos) ? "" : path.substr(0, idx + 1);
  };
  assert_eq("/home/", getDirpath("/home/a.txt"));
  assert_eq("/home/lz/", getDirpath("/home/lz/a.txt"));
  assert_eq("", getDirpath("a.txt"));
}

def(string_deep_or_shallow_copy) {
  // 都是 deep-copy. 不同gcc结果不同
  char a[10] = "123";
  string s(a);
  assert_neq(ptrToInt(a), ptrToInt(s.c_str()));
  string s1(s);
  assert_neq(ptrToInt(s.c_str()), ptrToInt(s1.c_str()));

  const int N = 3;
  TrivialInt arr[N];
  for (int i = 0; i < N; i++)
    arr[i].v = 0;

  // arr 到 bs 是深拷贝
  basic_string<TrivialInt> bs(arr, arr + 3);
  assert_neq(ptrToInt(arr), ptrToInt(bs.c_str()));

  // bs 到 bs1 是深拷贝
  basic_string<TrivialInt> bs1(bs);
  assert_neq(ptrToInt(bs.c_str()), ptrToInt(bs1.c_str()));
}

main_run;